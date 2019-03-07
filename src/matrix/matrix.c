#include "matrix/matrix.h"
#include <assert.h>
bvr_mat8_t *bvr_mat8_new(size_t width, size_t height)
{
    bvr_mat8_t *mat = (bvr_mat8_t *)malloc(sizeof(bvr_mat8_t));
    mat->content = (uint8_t *)calloc(width * height, sizeof(uint8_t));
    mat->width = width;
    mat->height = height;
    return mat;
}

bvr_mat32_t *bvr_mat32_new(size_t width, size_t height)
{
    bvr_mat32_t *mat = (bvr_mat32_t *)malloc(sizeof(bvr_mat32_t));
    mat->content = (uint32_t *)calloc(width * height, sizeof(uint32_t));
    mat->width = width;
    mat->height = height;
    return mat;
}

bvr_mat64_t *bvr_mat64_new(size_t width, size_t height)
{
    bvr_mat64_t *mat = (bvr_mat64_t *)malloc(sizeof(bvr_mat64_t));
    mat->content = (uint64_t *)calloc(width * height, sizeof(uint64_t));
    mat->width = width;
    mat->height = height;
    return mat;
}

bvr_matf64_t *bvr_matf64_new(size_t width, size_t height)
{

    bvr_matf64_t *mat = (bvr_matf64_t *)malloc(sizeof(bvr_matf64_t));
    mat->content = (double *)calloc(width * height, sizeof(double));
    mat->width = width;
    mat->height = height;
    return mat;
}

void bvr_matf_mul(bvr_matf64_t *lhs, bvr_matf64_t *rhs, bvr_matf64_t *result)
{
    assert(lhs->width == rhs->height);

    unsigned int i, j, k;
    double sum = 0;
    for (i = 0; i < lhs->height; i++)
    {
        for (j = 0; j < rhs->width; j++)
        {
            for (k = 0; k < rhs->height; k++)
            {
                sum += bvr_mat_get(lhs, k, i) * bvr_mat_get(rhs, j, k);
            }
            bvr_mat_set(result, j, i, sum);
            sum = 0;
        }
    }
}
