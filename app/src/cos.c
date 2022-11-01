/******************************************************************************
* File:             cos.c
*
* Author:           qrsforever
* Created:          11/01/22 
* Description:      
*****************************************************************************/

#include "cos_http_io.h"
#include "cos_api.h"
#include "cos_log.h"

static char *COS_SECRETID    = NULL;
static char *COS_SECRETKEY   = NULL;
static char *COS_ENDPOINT    = NULL;
static char *COS_APPID       = NULL;
static char *COS_BUCKET_NAME = NULL;
static char COS_OJB_URL[512] = {0};

void log_status(cos_status_t *s)
{
    cos_warn_log("status->code: %d", s->code);
    if (s->error_code) cos_warn_log("status->error_code: %s", s->error_code);
    if (s->error_msg) cos_warn_log("status->error_msg: %s", s->error_msg);
    if (s->req_id) cos_warn_log("status->req_id: %s", s->req_id);
}

int put_object_from_file(const char* local_path, const char* remote_path)
{
    cos_pool_t *p = NULL;
    cos_status_t *s = NULL;
    cos_request_options_t *options = NULL;
    cos_string_t bucket;
    cos_string_t object;
    cos_table_t *resp_headers;
    cos_string_t file;
    int ret = -1;
    cos_pool_create(&p, NULL);
    {
        options = cos_request_options_create(p);
        options->config = cos_config_create(options->pool);
        cos_str_set(&options->config->endpoint, COS_ENDPOINT);
        cos_str_set(&options->config->access_key_id, COS_SECRETID);
        cos_str_set(&options->config->access_key_secret, COS_SECRETKEY);
        cos_str_set(&options->config->appid, COS_APPID);
        options->ctl = cos_http_controller_create(options->pool, 0);
        cos_table_t *headers = NULL;
        cos_str_set(&bucket, COS_BUCKET_NAME);
        cos_str_set(&file, local_path);
        cos_str_set(&object, remote_path);
        s = cos_put_object_from_file(options, &bucket, &object, &file, headers, &resp_headers);
        log_status(s);
        if (s->code == 200)
            ret = 0;
    }
    cos_pool_destroy(p);
    return ret;
}


extern char* cosftpd_upload_file(const char *local_path, const char *remote_path)
{
    int ret = -1;
    if (COS_SECRETID == NULL) {
        COS_SECRETID    = getenv("COS_SECRETID");
        COS_SECRETKEY   = getenv("COS_SECRETKEY");
        COS_ENDPOINT    = getenv("COS_ENDPOINT");
        COS_APPID       = getenv("COS_APPID");
        COS_BUCKET_NAME = getenv("COS_BUCKET_NAME");
        sprintf(COS_OJB_URL, "https://%s.%s", COS_BUCKET_NAME, COS_ENDPOINT);
    }
    if (cos_http_io_initialize(NULL, 0) != COSE_OK)
        return NULL;
    cos_log_set_level(COS_LOG_WARN);
    cos_log_set_output(NULL);
    ret = put_object_from_file(local_path, remote_path);
    cos_http_io_deinitialize();
    if (ret < 0)
        return NULL;
    return COS_OJB_URL;
}
