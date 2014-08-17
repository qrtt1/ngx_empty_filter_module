#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

typedef struct {
    ngx_flag_t enable;
} ngx_http_funny_loc_conf_t;

static ngx_int_t 
ngx_http_funny_init(ngx_conf_t *cf);

static void *
ngx_http_funny_create_loc_conf(ngx_conf_t *cf);

static ngx_int_t
ngx_http_funny_header_filter(ngx_http_request_t *r);

static ngx_int_t
ngx_http_funny_body_filter(ngx_http_request_t *r, ngx_chain_t *in);

static ngx_command_t ngx_http_funny_commands [] = {
    { 
        ngx_string("funny"),
        NGX_HTTP_LOC_CONF|NGX_CONF_FLAG,
        ngx_conf_set_flag_slot,
        NGX_HTTP_LOC_CONF_OFFSET,
        offsetof(ngx_http_funny_loc_conf_t, enable),
        NULL
    },

    ngx_null_command
};

static ngx_http_module_t ngx_http_funny_module_ctx = {
    NULL,                                /* preconfiguration */
    ngx_http_funny_init,                /* postconfiguration */
    NULL,                                /* create main configuration */
    NULL,                                /* init main configuration */
    NULL,                                /* create server configuration */
    NULL,                                /* merge server configuration */
    ngx_http_funny_create_loc_conf,     /* create location configuration */
    NULL                                 /* merge location configuration */
};

ngx_module_t  ngx_http_funny_module = {
    NGX_MODULE_V1,
    &ngx_http_funny_module_ctx,         /* module context */
    ngx_http_funny_commands,            /* module directives */
    NGX_HTTP_MODULE,                     /* module type */
    NULL,                                /* init master */
    NULL,                                /* init module */
    NULL,                                /* init process */
    NULL,                                /* init thread */
    NULL,                                /* exit thread */
    NULL,                                /* exit process */
    NULL,                                /* exit master */
    NGX_MODULE_V1_PADDING
};

static ngx_http_output_header_filter_pt  ngx_http_next_header_filter;
static ngx_http_output_body_filter_pt    ngx_http_next_body_filter;

static ngx_int_t 
ngx_http_funny_init(ngx_conf_t *cf)
{
    ngx_http_next_header_filter = ngx_http_top_header_filter;
    ngx_http_top_header_filter = ngx_http_funny_header_filter;

    ngx_http_next_body_filter = ngx_http_top_body_filter;
    ngx_http_top_body_filter = ngx_http_funny_body_filter;

    return NGX_OK;
}

static void *
ngx_http_funny_create_loc_conf(ngx_conf_t *cf)
{
    ngx_http_funny_loc_conf_t  *lcf;

    lcf = ngx_pcalloc(cf->pool, sizeof(ngx_http_funny_loc_conf_t));
    if (lcf == NULL) {
        return NULL;
    }

    return lcf;
}

static ngx_int_t
ngx_http_funny_header_filter(ngx_http_request_t *r)
{
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "I SEE YOUR HEAD");
    return ngx_http_next_header_filter(r);
}

static ngx_int_t
ngx_http_funny_body_filter(ngx_http_request_t *r, ngx_chain_t *in)
{
    ngx_log_error(NGX_LOG_ERR, r->connection->log, 0, "I SEE YOUR BODY");
    return ngx_http_next_body_filter(r, in);
}
