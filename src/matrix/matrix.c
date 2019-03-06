#include "matrix/matrix.h"

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

bvr_matf32_t *bvr_matf32_new(size_t width, size_t height)
{

    bvr_matf32_t *mat = (bvr_matf32_t *)malloc(sizeof(bvr_matf32_t));
    mat->content = (double *)calloc(width * height, sizeof(double));
    mat->width = width;
    mat->height = height;
    return mat;
}