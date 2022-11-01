/******************************************************************************
* File:             http.c
*
* Author:           qrsforever  
* Created:          11/01/22 
* Description:      
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

static char *HTTP_SERVER_API = NULL;


extern int cosftpd_http_post(const char *jdata)
{
    if (HTTP_SERVER_API == NULL)
        HTTP_SERVER_API = getenv("HTTP_SERVER_API");

    CURL *curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, HTTP_SERVER_API);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jdata);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L); // 10s
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}
