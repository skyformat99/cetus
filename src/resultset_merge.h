#ifndef __RESULTSET_MERGE_H__
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <glib.h>

#include "sql-expression.h"
#include "network-mysqld.h"
#include "network-mysqld-proto.h"


#define MAX_NAME_LEN 64 
#define MAX_ORDER_COLS 16 
#define MAX_GROUP_COLS 16 
#define MAX_LIMIT G_MAXINT32
#define MAX_SHARD_NUM MAX_SERVER_NUM

typedef struct GROUP_BY {
    char table_name[MAX_NAME_LEN];
    char name[MAX_NAME_LEN];
    unsigned int desc;
    uint8_t type;
    int pos;
} GROUP_BY;

typedef struct ORDER_BY {
    char table_name[MAX_NAME_LEN];
    char name[MAX_NAME_LEN];
    unsigned int desc;
    unsigned int type;
    int pos;
} ORDER_BY;

typedef struct {
    GList *record;
    int index;
    unsigned int is_over:1;
    unsigned int is_err:1;
    unsigned int refreshed:1;
    unsigned int is_dup:1;
    unsigned int is_prior_to:1;
} heap_element;

typedef struct order_by_para_s {
    ORDER_BY order_array[MAX_ORDER_COLS];
    uint64_t field_index[MAX_SHARD_NUM];
    int order_array_size;
} order_by_para_t;

typedef struct {
    heap_element *element[MAX_SHARD_NUM];
    order_by_para_t order_para;
    unsigned int len:16;
    unsigned int is_over:1;
    unsigned int is_err:1;
} heap_type;

typedef struct aggr_by_group_para_s {
    network_queue *send_queue;
    GPtrArray *recv_queues;
    LIMIT *limit;
    GROUP_BY *group_array;
    GROUP_AGGR *aggr_array;
    having_condition_t *hav_condi;
    short aggr_num;
    short group_array_size;
} aggr_by_group_para_t;

NETWORK_API int callback_merge(network_mysqld_con *, merge_parameters_t *, int);
NETWORK_API void resultset_merge(network_queue *,  GPtrArray *, network_mysqld_con *, 
        uint64_t *, result_merge_t *); 

NETWORK_API gint check_dist_tran_resultset(network_queue *recv_queue, network_mysqld_con *);

#endif
