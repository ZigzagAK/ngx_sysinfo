#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>

static void *
ngx_sysinfo_create_loc_conf(ngx_conf_t *cf);
static char *
ngx_sysinfo_merge_loc_conf(ngx_conf_t *cf, void *parent, void *child);


static ngx_int_t
ngx_sysinfo_status_handler(ngx_http_request_t *r);

static void
ngx_sysinfo_status_json_format(ngx_buf_t *b);


static char *
ngx_sysinfo_status(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);


struct ngx_sysinfo_loc_conf_s {
    ngx_uint_t dummy;
};
typedef struct ngx_sysinfo_loc_conf_s ngx_sysinfo_loc_conf_t;


static ngx_command_t ngx_sysinfo_commands[] = {
    { ngx_string("sysinfo"),
      NGX_HTTP_SRV_CONF|NGX_HTTP_LOC_CONF|NGX_CONF_NOARGS,
      ngx_sysinfo_status,
      0,
      0,
      NULL },

      ngx_null_command
};


static ngx_http_module_t ngx_sysinfo_module_ctx = {
    NULL,                        /* preconfiguration */
    NULL,                        /* postconfiguration */

    NULL,                        /* create main configuration */
    NULL,                        /* init main configuration */

    NULL,                        /* create server configuration */
    NULL,                        /* merge server configuration */

    ngx_sysinfo_create_loc_conf, /* create location configuration */
    ngx_sysinfo_merge_loc_conf   /* merge location configuration */
};


ngx_module_t ngx_sysinfo_module = {
    NGX_MODULE_V1,
    &ngx_sysinfo_module_ctx,   /* module context */
    ngx_sysinfo_commands,      /* module directives */
    NGX_HTTP_MODULE,           /* module type */
    NULL,                      /* init master */
    NULL,                      /* init module */
    NULL,                      /* init process */
    NULL,                      /* init thread */
    NULL,                      /* exit thread */
    NULL,                      /* exit process */
    NULL,                      /* exit master */
    NGX_MODULE_V1_PADDING
};


static void *
ngx_sysinfo_create_loc_conf(ngx_conf_t *cf)
{
    ngx_sysinfo_loc_conf_t *uclcf;

    uclcf = ngx_pcalloc(cf->pool, sizeof(ngx_sysinfo_loc_conf_t));
    if (uclcf == NULL) {
        return NULL;
    }

    return uclcf;
}


static char *
ngx_sysinfo_merge_loc_conf(ngx_conf_t *cf, void *parent,
    void *child)
{
/*
    ngx_sysinfo_loc_conf_t *prev = parent;
    ngx_sysinfo_loc_conf_t *conf = child;
*/
    return NGX_CONF_OK;
}


static char *
ngx_sysinfo_status(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
{
    ngx_http_core_loc_conf_t *clcf;
    clcf = ngx_http_conf_get_module_loc_conf(cf, ngx_http_core_module);
    clcf->handler = ngx_sysinfo_status_handler;
    return NGX_CONF_OK;
}

static ngx_str_t
content_type = ngx_string("application/json");

static ngx_int_t
ngx_sysinfo_status_handler(ngx_http_request_t *r)
{
    size_t                  buffer_size;
    ngx_int_t               rc;
    ngx_buf_t              *b;
    ngx_chain_t             out;
//  ngx_sysinfo_loc_conf_t *uclcf;

    if (r->method != NGX_HTTP_GET) {
        return NGX_HTTP_NOT_ALLOWED;
    }

    rc = ngx_http_discard_request_body(r);

    if (rc != NGX_OK) {
        return rc;
    }

//  uclcf = ngx_http_get_module_loc_conf(r, ngx_sysinfo_module);

    r->headers_out.content_type = content_type;

    buffer_size = ngx_align(1024 * 1024, ngx_pagesize) + ngx_pagesize;

    b = ngx_create_temp_buf(r->pool, buffer_size);
    if (b == NULL) {
        return NGX_HTTP_INTERNAL_SERVER_ERROR;
    }

    out.buf = b;
    out.next = NULL;

    ngx_sysinfo_status_json_format(b);

    r->headers_out.status = NGX_HTTP_OK;
    r->headers_out.content_length_n = b->last - b->pos;

    if (r->headers_out.content_length_n == 0) {
        r->header_only = 1;
    }

    b->last_buf = 1;

    rc = ngx_http_send_header(r);

    if (rc == NGX_ERROR || rc > NGX_OK || r->header_only) {
        return rc;
    }

    return ngx_http_output_filter(r, &out);
}

static void
ngx_sysinfo_status_json_format(ngx_buf_t *b)
{
    ngx_uint_t        i, count;
    ngx_shm_zone_t   *shm_zone;
    ngx_slab_pool_t  *shpool;
    ngx_slab_page_t  *page;
    ngx_list_part_t  *part;
    size_t            free_pages, total_pages;
#if defined(nginx_version) && (nginx_version >= 1011007)
    size_t            sum_size;
    size_t            slot_count;
    size_t            slot_size;
    ngx_slab_stat_t  *stat;
#endif

    b->last = ngx_snprintf(b->last, b->end - b->last,
                           "{\"shared_memory\": [");

    part = (ngx_list_part_t *) &(ngx_cycle->shared_memory.part);
    shm_zone = part->elts;

    count = 0;

    for (i = 0; /* void */ ; i++) {
        if (i >= part->nelts) {
            if (part->next == NULL) {
                break;
            }
            part = part->next;
            shm_zone = part->elts;
            i = 0;
        }

        shpool = (ngx_slab_pool_t *) shm_zone[i].shm.addr;
        free_pages = 0;
        total_pages = (shpool->end - shpool->start) / ngx_pagesize;

        count++;

        ngx_shmtx_lock(&shpool->mutex);

        for (page = shpool->free.next; page != &shpool->free; page = page->next) {
            free_pages += page->slab;
        }

#if defined(nginx_version) && (nginx_version >= 1011007)
        b->last = ngx_snprintf(b->last, b->end - b->last,
                               "{\"name\": \"%s\",\"pages\":%l,\"free_pages\":%l,\"used\":\"%l%%\",\"slab\":[",
                               shm_zone[i].shm.name.data, total_pages, free_pages, 100 * (total_pages - free_pages) / total_pages);

        slot_count = ngx_pagesize_shift - shpool->min_shift;
        stat = shpool->stats;
        slot_size = shpool->min_size;
        sum_size = 0;

        for (i = 0; i < slot_count; i++) {
            sum_size += stat->used * slot_size;
            b->last = ngx_snprintf(b->last, b->end - b->last,
                                   "{\"size\":%l,\"used\":%l,\"total\":%l,\"fails\":%l},",
                                   slot_size, stat->used * slot_size, stat->total * slot_size, stat->fails);
            stat++;
            slot_size <<= 1;
        }

        if (slot_count != 0) {
            b->last--;
        }

        b->last = ngx_snprintf(b->last, b->end - b->last, "],\"summary\":{\"total\":%l,\"used\":%l}},", total_pages * ngx_pagesize, sum_size);
#else
        b->last = ngx_snprintf(b->last, b->end - b->last,
                               "{\"name\": \"%s\",\"pages\":%l,\"free_pages\":%l,\"used\":\"%l%%\"},",
                               shm_zone[i].shm.name.data, total_pages, free_pages, 100 * (total_pages - free_pages) / total_pages);
#endif
        ngx_shmtx_unlock(&shpool->mutex);
    }

    if (count != 0) {
        b->last--;
    }

    b->last = ngx_snprintf(b->last, b->end - b->last, "]}");
}
