#ifndef __BVR_SCAN_CORE_H
#define __BVR_SCAN_CORE_H

#include <stdlib.h>
#include <stdio.h>
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
} bvr_mat_real_t;

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
bvr_mat8_t *bvr_mat8_new(size_t rows, size_t columns);

/**
 * @brief Creates an empty `uint8_t` matrix
 * 
 * @param columns The columns of the matrix (column)
 * @param rows The rows of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_mat32_t *bvr_mat32_new(size_t rows, size_t columns);

/**
 * @brief Creates an empty `uint32_t` matrix
 * 
 * @param columns The columns of the matrix (column)
 * @param rows The rows of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_mat_real_t *bvr_mat_real_new(size_t rows, size_t columns);

/**
 * @brief Creates an empty `uint64_t` matrix
 * 
 * @param columns The columns of the matrix (column)
 * @param rows The rows of the matrix (rows)
 * @return bvr_mat8_t* 
 */

bvr_mat64_t *bvr_mat64_new(size_t rows, size_t columns);

void bvr_mat_real_load(bvr_mat_real_t *mat, double *array);

#define bvr_mat_free(m) \
  do                    \
  {                     \
    free(m->content);   \
    free(m);            \
  } while (0)

#define bvr_mat_get(m, r, c) \
  m->content[(c) + (r)*m->columns]

#define bvr_mat_set(m, r, c, val) \
  m->content[(c) + (r)*m->columns] = val

#define bvr_mat_print(m)                          \
  do                                              \
  {                                               \
    int row, col;                                 \
    for (row = 0; row < m->rows; row++)           \
    {                                             \
      for (col = 0; col < m->columns; col++)      \
      {                                           \
        printf("%i\t", bvr_mat_get(m, row, col)); \
      }                                           \
      printf("\n");                               \
    }                                             \
  } while (0)

; /**
 * @brief Matrix multiplication
 * 
 * The result matrix size should be set accordingly before calling this function:
 * lhs[m,n], rhs[p,q] -> result[m,q]
 * 
 * @param lhs The left matrix
 * @param rhs The right matrix
 * @param result The matrix where to store the result
 */
void bvr_mat_real_mul(const bvr_mat_real_t *lhs, const bvr_mat_real_t *rhs, bvr_mat_real_t *result);

/**
 * @brief Multiply the matrix by a scalar
 * 
 * The size of the result matrix should be the same as mat
 * 
 * @param mat The matrix operand
 * @param scalar The scalar operand
 * @param result The matrix where to store the result
 */
void bvr_mat_scalar_mul(const bvr_mat_real_t *mat, double scalar, bvr_mat_real_t *result);

void bvr_mat_add(const bvr_mat_real_t *lhs, const bvr_mat_real_t *rhs, bvr_mat_real_t *result);

void bvr_mat_transpose(const bvr_mat_real_t *lhs, bvr_mat_real_t *result);
/**
 * @brief A pointer of function that takes a double and return a double
 * 
 */
typedef double (*mat_scalar_func)(double);

void bvr_mat_apply_scalar_func(const bvr_mat_real_t *mat, mat_scalar_func f, bvr_mat_real_t *result);

bvr_mat_real_t *bvr_mat_real_from_mat8(const bvr_mat8_t *src);
#endif
