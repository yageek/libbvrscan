#ifndef __BVR_CORE_BLOB_H
#define __BVR_CORE_BLOB_H

#include "matrix/matrix.h"

#include <stdlib.h>

typedef struct
{
    size_t x_min, x_max, y_min, y_max;
} bvr_blob_t;

bvr_mat32_t *bvr_filter_create_vertical_proj_mat(const bvr_mat8_t *src);
bvr_mat32_t *bvr_filter_create_horizontal_proj_mat(const bvr_mat8_t *src);

int bvr_blobs_flood_fill(const bvr_mat8_t *src, uint8_t searched, uint8_t replace, bvr_blob_t **array, size_t *array_len);
int bvr_blobs_projections(const bvr_mat8_t *src, bvr_blob_t **array, size_t *array_len);
#endif
