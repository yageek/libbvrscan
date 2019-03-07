#ifndef __BVR_SCAN_CORE_H
#define __BVR_SCAN_CORE_H

#include <stdlib.h>

typedef struct
{
  size_t width;
  size_t height;
  uint8_t *content;
} bvr_mat8_t;

typedef struct
{
  size_t width;
  size_t height;
  uint32_t *content;
} bvr_mat32_t;

typedef struct
{
  size_t width;
  size_t height;
  float *content;
} bvr_matf32_t;

typedef struct
{
  size_t width;
  size_t height;
  uint64_t *content;
} bvr_mat64_t;

/**
 * @brief Creates an empty `uint8_t` matrix
 * 
 * @param width The width of the matrix (column)
 * @param height The height of the matrix (rows)
 * @return bvr_mat8_t* 
 */
bvr_mat8_t *bvr_mat8_new(size_t width, size_t height);

/**
 * @brief Creates an empty `uint8_t` matrix
 * 
 * @param width The width of the matrix (column)
 * @param height The height of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_mat32_t *bvr_mat32_new(size_t width, size_t height);

/**
 * @brief Creates an empty `uint32_t` matrix
 * 
 * @param width The width of the matrix (column)
 * @param height The height of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_matf32_t *bvr_matf32_new(size_t width, size_t height);

/**
 * @brief Creates an empty `uint64_t` matrix
 * 
 * @param width The width of the matrix (column)
 * @param height The height of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_mat64_t *bvr_mat64_new(size_t width, size_t height);

#define bvr_mat_free(m) \
  do                    \
  {                     \
    free(m->content);   \
    free(m);            \
  } while (0)

#define bvr_mat_get(m, w, h) \
  m->content[(w) + (h)*m->width]

#define bvr_mat_set(m, w, h, val) \
  m->content[(w) + (h)*m->width] = val

int bvr_matf_mul(bvr_mat32_t *lhs, bvr_mat32_t *rhs, bvr_mat32_t *result);
#endif
