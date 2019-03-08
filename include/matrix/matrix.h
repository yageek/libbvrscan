#ifndef __BVR_SCAN_CORE_H
#define __BVR_SCAN_CORE_H

#include <stdlib.h>

typedef struct
{
  size_t columns;
  size_t rows;
  uint8_t *content;
} bvr_mat8_t;

typedef struct
{
  size_t columns;
  size_t rows;
  uint32_t *content;
} bvr_mat32_t;

typedef struct
{
  size_t columns;
  size_t rows;
  double *content;
} bvr_matf64_t;

typedef struct
{
  size_t columns;
  size_t rows;
  uint64_t *content;
} bvr_mat64_t;

/**
 * @brief Creates an empty `uint8_t` matrix
 * 
 * @param columns The columns of the matrix (column)
 * @param rows The rows of the matrix (rows)
 * @return bvr_mat8_t* 
 */
bvr_mat8_t *bvr_mat8_new( size_t rows,size_t columns);

/**
 * @brief Creates an empty `uint8_t` matrix
 * 
 * @param columns The columns of the matrix (column)
 * @param rows The rows of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_mat32_t *bvr_mat32_new( size_t rows,size_t columns);

/**
 * @brief Creates an empty `uint32_t` matrix
 * 
 * @param columns The columns of the matrix (column)
 * @param rows The rows of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_matf64_t *bvr_matf64_new( size_t rows,size_t columns);

/**
 * @brief Creates an empty `uint64_t` matrix
 * 
 * @param columns The columns of the matrix (column)
 * @param rows The rows of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_mat64_t *bvr_mat64_new( size_t rows,size_t columns);

#define bvr_mat_free(m) \
  do                    \
  {                     \
    free(m->content);   \
    free(m);            \
  } while (0)

#define bvr_mat_get(m, c, r) \
  m->content[(c) + (r)*m->columns]

#define bvr_mat_set(m, c, r, val) \
  m->content[(c) + (r)*m->columns] = val

void bvr_matf_mul(bvr_matf64_t *lhs, bvr_matf64_t *rhs, bvr_matf64_t *result);
#endif
