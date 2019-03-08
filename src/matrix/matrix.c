#include "matrix/matrix.h"
#include <assert.h>

bvr_mat8_t *bvr_mat8_new(size_t rows, size_t columns)
{
    bvr_mat8_t *mat = (bvr_mat8_t *)malloc(sizeof(bvr_mat8_t));
    mat->content = (uint8_t *)calloc(columns * rows, sizeof(uint8_t));
    mat->columns = columns;
    mat->rows = rows;
    return mat;
}

bvr_mat32_t *bvr_mat32_new(size_t rows, size_t columns)
{
    bvr_mat32_t *mat = (bvr_mat32_t *)malloc(sizeof(bvr_mat32_t));
    mat->content = (uint32_t *)calloc(columns * rows, sizeof(uint32_t));
    mat->columns = columns;
    mat->rows = rows;
    return mat;
}

bvr_mat64_t *bvr_mat64_new(size_t rows, size_t columns)
{
    bvr_mat64_t *mat = (bvr_mat64_t *)malloc(sizeof(bvr_mat64_t));
    mat->content = (uint64_t *)calloc(columns * rows, sizeof(uint64_t));
    mat->columns = columns;
    mat->rows = rows;
    return mat;
}

bvr_matf64_t *bvr_matf64_new(size_t rows, size_t columns)
{

    bvr_matf64_t *mat = (bvr_matf64_t *)malloc(sizeof(bvr_matf64_t));
    mat->content = (double *)calloc(columns * rows, sizeof(double));
    mat->columns = columns;
    mat->rows = rows;
    return mat;
}

void bvr_matf_mul(const bvr_matf64_t *lhs, const bvr_matf64_t *rhs, bvr_matf64_t *result)
{
    assert(lhs->columns == rhs->rows);

    unsigned int i, j, k;
    double sum = 0;
    for (i = 0; i < lhs->rows; i++)
    {
        for (j = 0; j < rhs->columns; j++)
        {
            for (k = 0; k < rhs->rows; k++)
            {
                sum += bvr_mat_get(lhs, i, k) * bvr_mat_get(rhs, k, j);
            }
            bvr_mat_set(result, i, j, sum);
            sum = 0;
        }
    }
}

void bvr_mat_scalar_mul(const bvr_matf64_t *mat, double scalar, bvr_matf64_t *result)
{
    int row, col;
    for (row = 0; row < mat->rows; row++)
    {
        for (col = 0; col < mat->columns; col++)
        {
            double val = bvr_mat_get(mat, row, col) * scalar;
            bvr_mat_set(result, row, col, val);
        }
    }
}

void bvr_mat_apply_scalar_func(const bvr_matf64_t *mat, mat_scalar_func f, bvr_matf64_t *result)
{
    int row, col;
    for (row = 0; row < mat->rows; row++)
    {
        for (col = 0; col < mat->columns; col++)
        {
            double val = f(bvr_mat_get(mat, row, col));
            bvr_mat_set(result, row, col, val);
        }
    }
}
