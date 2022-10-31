#include "cos_http_io.h"
#include "cos_api.h"
#include "cos_log.h"

// region = 'ap-beijing'
// bucket = 'frepai'
// bucket_name = f'{bucket}-1301930378'
// access_key = os.environ.get('MINIO_ACCESS_KEY', 'AKIDV7XjgOr42nMhneGdmiPs66rNioeFafeT')
// secret_key = os.environ.get('MINIO_SECRET_KEY', 'd190cxQk0CHCtLXjhQt65tUr2yf7KI1V')
// coss3_domain = f'https://{bucket_name}.cos.{region}.myqcloud.com'

static char TEST_COS_ENDPOINT[] = "cos.ap-beijing.myqcloud.com";
static char *TEST_ACCESS_KEY_ID;                //your secret_id
static char *TEST_ACCESS_KEY_SECRET;            //your secret_key
static char TEST_APPID[] = "1301930378";    //your appid
//the cos bucket name, syntax: [bucket]-[appid], for example: mybucket-1253666666，可在 https://console.cloud.tencent.com/cos5/bucket 查看
static char TEST_BUCKET_NAME[] = "frepai-1301930378";
static char TEST_OBJECT_NAME1[] = "a/b/c/1.txt";
static char TEST_OBJECT_NAME2[] = "c/b/a/1.txt";


void log_status(cos_status_t *s)
{
    cos_warn_log("status->code: %d", s->code);
    if (s->error_code) cos_warn_log("status->error_code: %s", s->error_code);
    if (s->error_msg) cos_warn_log("status->error_msg: %s", s->error_msg);
    if (s->req_id) cos_warn_log("status->req_id: %s", s->req_id);
}

void init_test_config(cos_config_t *config, int is_cname)
{
    cos_str_set(&config->endpoint, TEST_COS_ENDPOINT);
    cos_str_set(&config->access_key_id, TEST_ACCESS_KEY_ID);
    cos_str_set(&config->access_key_secret, TEST_ACCESS_KEY_SECRET);
    cos_str_set(&config->appid, TEST_APPID);
    config->is_cname = is_cname;
}

void init_test_request_options(cos_request_options_t *options, int is_cname)
{
    options->config = cos_config_create(options->pool);
    init_test_config(options->config, is_cname);
    options->ctl = cos_http_controller_create(options->pool, 0);
}

void test_put_object_from_file()
{
    cos_pool_t *p = NULL;
    int is_cname = 0;
    cos_status_t *s = NULL;
    cos_request_options_t *options = NULL;
    cos_string_t bucket;
    cos_string_t object;
    cos_table_t *resp_headers;
    cos_string_t file;

    cos_pool_create(&p, NULL);
    options = cos_request_options_create(p);
    init_test_request_options(options, is_cname);
    cos_table_t *headers = NULL;
    cos_str_set(&bucket, TEST_BUCKET_NAME);
    cos_str_set(&file, TEST_OBJECT_NAME1);
    cos_str_set(&object, TEST_OBJECT_NAME2);
    s = cos_put_object_from_file(options, &bucket, &object, &file, headers, &resp_headers);
    log_status(s);

    cos_pool_destroy(p);
}

int main(int argc, char *argv[])
{
    // 通过环境变量获取 SECRETID 和 SECRETKEY
    TEST_ACCESS_KEY_ID     = "AKIDV7XjgOr42nMhneGdmiPs66rNioeFafeT"; // getenv("COS_SECRETID");
    TEST_ACCESS_KEY_SECRET = "d190cxQk0CHCtLXjhQt65tUr2yf7KI1V"; // getenv("COS_SECRETKEY");

    if (cos_http_io_initialize(NULL, 0) != COSE_OK) {
       exit(1);
    }

    //set log level, default COS_LOG_WARN
    cos_log_set_level(COS_LOG_WARN);

    //set log output, default stderr
    cos_log_set_output(NULL);

    test_put_object_from_file();

    cos_http_io_deinitialize();

    return 0;
}
