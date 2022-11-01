/******************************************************************************
* File:             main.c
*
* Author:           qrsforever
* Created:          10/28/22
* Description:
*****************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/wait.h>

extern char *cosftpd_upload_file(const char *lpath, const char *rpath);
extern int cosftpd_http_post(const char *jdata);

static char *FTP_USER = NULL;
#define FTP_ROOT_PATH "/home/vsftpd"
#define HH_FIFO_PATH  "/run/vsftpd.fifo"
#define MSG_LEN 256

static int g_msg_writer = -1;

void catch_signal(int sig)
{
    printf("catch sig: %d, msg writer: %d", sig, g_msg_writer);
    if (g_msg_writer > 0) {
        char buff[MSG_LEN] = {"quit"};
        write(g_msg_writer, buff, MSG_LEN);
    }
    sleep(2);
    if (access(HH_FIFO_PATH, F_OK) == 0)
        unlink(HH_FIFO_PATH);
    exit(0);
}

void *handle_message(void *arg)
{
    int ret = -1;
    char* image_file_path = (char*)arg;
    if (FTP_USER == NULL)
        FTP_USER = getenv("FTP_USER");
    if (image_file_path != NULL) {
        printf("handle_message: %s\n", image_file_path);
        char local_path[512] = {0};
        char remote_path[512] = {0};
        sprintf(local_path, "%s/%s/%s", FTP_ROOT_PATH, FTP_USER, image_file_path);
        ret = access(local_path, F_OK);
        if (ret == 0) {
            sprintf(remote_path, "%s%s", FTP_USER, image_file_path);
            char *cos_url = cosftpd_upload_file(local_path, remote_path);
            if (ret == 0) {
                printf("remove local file: %s\n", local_path);
                unlink(local_path);
                const char *endstr = strchr(image_file_path + 1, '/');
                char mac[32] = {0};
                memcpy(mac, image_file_path + 1, endstr - image_file_path - 1);
                char jdata[1024] = {0};
                sprintf(
                    jdata,
                    "{\"ftp_user\":\"%s\", \"mac\":\"%s\", \"cos_url\":\"%s/%s\"}",
                    FTP_USER, mac, cos_url, remote_path);
                cosftpd_http_post(jdata);
            }
        }
        free(image_file_path);
    }
    return NULL;
}


int main(void)
{
    printf("Start\n");
    int ret = access(HH_FIFO_PATH, F_OK);
    if (-1 == ret) {
        ret = mkfifo(HH_FIFO_PATH, 0664);
        if (-1 == ret) {
            perror("mkfifo");
            exit(-1);
        }
    }

    int cos_pid = fork();
    if (cos_pid > 0) {
        int ftpd_pid = fork();
        if (ftpd_pid > 0) {
            printf("parent process start...\n");
            signal(SIGINT, catch_signal);
            signal(SIGTERM, catch_signal);
            int fw = open(HH_FIFO_PATH, O_WRONLY);
            if (-1 == fw) {
                perror("open");
                exit(-1);
            }
            g_msg_writer = fw;
            while(wait(NULL) > 0);
        } else if (ftpd_pid == 0) {
            printf("ftpd process start...\n");
            execl("/app/bin/vsftpd", "vsftpd", "/app/config/vsftpd.conf", NULL);
        } else {
            perror("ftpd fork");
            exit(-1);
        }
    } else if (cos_pid == 0) {
        printf("cos process start...\n");
        int fr = open(HH_FIFO_PATH, O_RDONLY);
        if (-1 == fr) {
            perror("open");
            exit(-1);
        }
        while (1) {
            char buff[MSG_LEN] = {0};
            int len = read(fr, buff, MSG_LEN);
            if (len == 0) {
                perror("read");
                break;
            }
            printf("recv: %s\n", buff);
            if (strcmp(buff, "quit") == 0)
                break;
            char* msg = (char*)malloc(MSG_LEN);
            memcpy(msg, buff, MSG_LEN);
            pthread_t thread;
            int ret = pthread_create(&thread, NULL, handle_message, (void*)msg);
            if (ret != 0) {
                perror("pthread_create");
                continue;
            }
            pthread_detach(thread);
        }
        close(fr);
        unlink(HH_FIFO_PATH);
    } else {
        perror("cos fork");
        exit(-1);
    }
    return 0;
}
