#ifndef __BVR_CORE_BLOB_PRIVATE_H
#define __BVR_CORE_BLOB_PRIVATE_H
#include <stdlib.h>

#define __QUEUE_DEFAULT_SIZE 20

#define _MAX(x, y) (((x) > (y)) ? (x) : (y))
#define _MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct
{
    int x, y;
} __bvr_blob_node_t;

typedef struct
{
    __bvr_blob_node_t *content;
    size_t cap;
    __bvr_blob_node_t *head;
    __bvr_blob_node_t *tail;

} __bvr_blob_node_queue_t;

__bvr_blob_node_queue_t _bvr_blob_new_queue();

void _bvr_blob_queue_push(__bvr_blob_node_queue_t *queue, __bvr_blob_node_t node);
void _bvr_blob_queue_pop(__bvr_blob_node_queue_t *queue);
void _bvr_blob_queue_clear(__bvr_blob_node_queue_t *queue);
int _bvr_blob_queue_is_empty(__bvr_blob_node_queue_t *queue);
#endif