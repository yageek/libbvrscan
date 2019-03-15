#ifndef __BVR_CORE_BLOB_H
#define __BVR_CORE_BLOB_H

#include "matrix/matrix.h"

#include <stdlib.h>

/**
 * @brief A blob inside the matrix/image
 * 
 */
typedef struct
{
    size_t x_min, x_max, y_min, y_max;
} bvr_blob_t;

/**
 * @brief Compute the vertical projection of a grayscale matrix
 * 
 * @param src 
 * @return bvr_mat32_t* 
 */
bvr_mat32_t *bvr_filter_create_vertical_proj_mat(const bvr_mat8_t *src);

/**
 * @brief Computes the horizontal projection of a grayscale matrix
 * 
 * @param src 
 * @return bvr_mat32_t* 
 */
bvr_mat32_t *bvr_filter_create_horizontal_proj_mat(const bvr_mat8_t *src);

/**
 * @brief Performs a blob detection insde a grayscale image using the flood fil algorithm
 * 
 * @param src The input grayscale image
 * @param searched The searched value as foreground
 * @param replace the value used to replace the foreground
 * @param array The blobs array where to store the result
 * @param array_len The array length of the returned array
 * @return int 
 */
int bvr_blobs_flood_fill(const bvr_mat8_t *src, uint8_t searched, uint8_t replace, bvr_blob_t **array, size_t *array_len);
int bvr_blobs_projections(const bvr_mat8_t *src, bvr_blob_t **array, size_t *array_len);


bvr_mat8_t *bvr_extract_blob(const bvr_mat8_t *src, const bvr_blob_t *blob);
#endif
