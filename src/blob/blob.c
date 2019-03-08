#include "blob/blob.h"
#include "filter/filter.h"
#include "bvrcore_private.h"
#include "matrix/matrix.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

__bvr_blob_node_queue_t _bvr_blob_new_queue()
{
    __bvr_blob_node_queue_t queue;
    queue.cap = __QUEUE_DEFAULT_SIZE;
    queue.head = NULL;
    queue.tail = NULL;
    queue.content = malloc(__QUEUE_DEFAULT_SIZE * sizeof(bvr_blob_t));
    return queue;
}

void _bvr_blob_queue_push(__bvr_blob_node_queue_t *queue, __bvr_blob_node_t node)
{
    if (queue->head == NULL && queue->tail == NULL)
    {
        queue->head = &queue->content[0];
        queue->tail = &queue->content[0];
        queue->head[0] = node;
        return;
    }

    __bvr_blob_node_t *next = queue->head + 1;
    size_t len = &queue->content[queue->cap] - next;
    if (len > queue->cap)
    {
        queue->content = realloc(queue->content, len);
    }

    *next = node;
    queue->head = next;
}

void _bvr_blob_queue_pop(__bvr_blob_node_queue_t *queue)
{
    if (queue->head == NULL && queue->tail == NULL)
        return;

    if (queue->tail < queue->head)
    {
        queue->tail++;
        return;
    }

    if (queue->tail == queue->head)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
}

void _bvr_blob_queue_clear(__bvr_blob_node_queue_t *queue)
{
    queue->head = NULL;
    queue->tail = NULL;
    memset(queue->content, queue->cap, 0);
}

int _bvr_blob_queue_is_empty(__bvr_blob_node_queue_t *queue)
{
    return queue->head == NULL && queue->tail == NULL;
}

int bvr_blobs_flood_fill(const bvr_mat8_t *src, uint8_t searched, uint8_t replace, bvr_blob_t **array, size_t *array_len)
{
    uint8_t blob = 0;

    bvr_mat8_t *tmp = bvr_mat8_new(src->columns, src->rows);
    memcpy(tmp->content, src->content, src->columns * src->rows * sizeof(uint8_t));

    size_t blobs_count = __QUEUE_DEFAULT_SIZE;
    size_t blobs_index = 0;
    bvr_blob_t *blobs = malloc(blobs_count * sizeof(bvr_blob_t));

    __bvr_blob_node_queue_t queue_store = _bvr_blob_new_queue();
    __bvr_blob_node_queue_t *queue = &queue_store;

    int xMin = xMin, xMax, yMin, yMax;
    for (int y = 0; y < tmp->rows; y++)
    {
        for (int x = 0; x < tmp->columns; x++)
        {
            if (bvr_mat_get(tmp, x, y) == searched)
            {
                xMin = x;
                xMax = x;
                yMin = y;
                yMax = y;

                // Flood-fill starts here
                _bvr_blob_queue_clear(queue);

                __bvr_blob_node_t node = (__bvr_blob_node_t){x, y};
                _bvr_blob_queue_push(queue, node);

                while (!_bvr_blob_queue_is_empty(queue))
                {
                    __bvr_blob_node_t n = *queue->head;
                    _bvr_blob_queue_pop(queue);

                    int w = n.x;
                    int e = n.x;

                    while (w > 0 && bvr_mat_get(tmp, w, n.y) == searched)
                        w--;

                    while (e < tmp->columns && bvr_mat_get(tmp, e, n.y) == searched)
                        e++;

                    xMin = _MIN(xMin, w + 1);
                    xMax = _MAX(xMax, e - 1);
                    yMin = _MIN(yMin, n.y);
                    yMax = _MAX(yMax, n.y);

                    for (int i = w + 1; i < e; i++)
                    {
                        int top = n.y - 1;
                        int bottom = n.y + 1;
                        if (n.y > 1 && bvr_mat_get(tmp, i, top) == searched)
                        {
                            __bvr_blob_node_t node = (__bvr_blob_node_t){i, top};
                            _bvr_blob_queue_push(queue, node);
                        }

                        if (n.y < tmp->rows - 1 && bvr_mat_get(tmp, i, bottom) == searched)
                        {
                            __bvr_blob_node_t node = (__bvr_blob_node_t){i, bottom};
                            _bvr_blob_queue_push(queue, node);
                        }

                        // Treat point
                        bvr_mat_set(tmp, i, n.y, replace);
                    }
                }
                blob++;
                bvr_blob_t found = (bvr_blob_t){xMin, xMax, yMin, yMax};

                if (blobs_index >= blobs_count)
                {
                    size_t new = blobs_count + __QUEUE_DEFAULT_SIZE;
                    blobs = realloc(blobs, new);
                    blobs_count = new;
                }

                blobs[blobs_index++] = found;
            }
        }
    }

    free(queue->content);
    *array = blobs;
    *array_len = blob;

    return 0;
}

typedef struct
{
    size_t len;
    size_t element_size;
    size_t cap;
    void *content;

} _bvr_array_t;

_bvr_array_t _bvr_new_array(size_t element_size)
{

    void *content = malloc(element_size * 12);
    return (_bvr_array_t){0, element_size, 12, content};
}

void _bvr_array_free(_bvr_array_t array)
{
    free(array.content);
}

#define _bvr_array_get(array, i) \
    *(&array.content[0] + ((i)*array.element_size))

#define _bvr_array_set(array, i, val)                         \
    void *ptr = &array.content[0] + ((i)*array.element_size); \
    memcpy(ptr, &i, array.element_size);

#define _bvr_array_push(array, val)                                 \
    do                                                              \
    {                                                               \
        if ((array.len + 1) > array.cap)                            \
        {                                                           \
            array.content = realloc(array.content, array.cap + 12); \
            array.cap += 12;                                        \
        }                                                           \
        _bvr_array_set(array, array.len - 1, val);                  \
        array.len++;                                                \
    } while (0)

int bvr_blobs_projections(const bvr_mat8_t *src, bvr_blob_t **array, size_t *array_len)
{

    bvr_mat32_t *horizontal_proj = bvr_filter_create_horizontal_proj_mat(src);
    bvr_mat32_t *vertical_proj = bvr_filter_create_vertical_proj_mat(src);
    // First top - bottom
    bool previousInPeek = false,
         currentInPeek = false;
    int top = -1, bottom = -1, i;
    for (i = 0; i < horizontal_proj->columns; i++)
    {

        uint32_t total = bvr_mat_get(horizontal_proj, i, 0);
        currentInPeek = total >= 1;

        if (currentInPeek && !previousInPeek)
        {
            top = i;
        }
        if (!currentInPeek && previousInPeek)
        {
            bottom = i - 1;
        }

        if (top != -1 && bottom != -1)
        {
            break;
        }
        previousInPeek = currentInPeek;
    }

    _bvr_array_t blobs = _bvr_new_array(sizeof(bvr_blob_t));
    previousInPeek = false;
    currentInPeek = false;

    int left = 0, right = 0;
    for (i = 0; i < vertical_proj->columns; i++)
    {
        uint32_t total = bvr_mat_get(vertical_proj, i, 0);
        currentInPeek = (total >= 1);

        // Starts character
        if (currentInPeek && !previousInPeek)
        {
            left = i;
        }
        if (!currentInPeek && previousInPeek)
        {
            right = i - 1;

            bvr_blob_t bl = (bvr_blob_t){left, right, top, bottom};
            _bvr_array_push(blobs, bl);
        }
        previousInPeek = currentInPeek;
    }

    bvr_mat_free(vertical_proj);
    bvr_mat_free(horizontal_proj);

    *array = blobs.content;
    *array_len = blobs.len;
    return 0;
}

bvr_mat32_t *bvr_filter_create_vertical_proj_mat(const bvr_mat8_t *src)
{
    bvr_mat32_t *out = bvr_mat32_new(src->columns, 1);
    for (int x = 0; x < src->columns; x++)
    {
        uint32_t total = 0;
        for (int y = 0; y < src->rows; y++)
        {
            total += bvr_mat_get(src, x, y);
        }
        bvr_mat_set(out, x, 0, total);
    }
    return out;
}

bvr_mat32_t *bvr_filter_create_horizontal_proj_mat(const bvr_mat8_t *src)
{
    bvr_mat32_t *out = bvr_mat32_new(src->rows, 1);

    for (int y = 0; y < src->rows; y++)
    {
        uint32_t total = 0;
        for (int x = 0; x < src->columns; x++)
        {
            total += bvr_mat_get(src, x, y);
        }
        bvr_mat_set(out, y, 0, total);
    }
    return out;
}
