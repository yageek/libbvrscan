#include "filter/filter.h"
#include "matrix/matrix.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

// Code used from
bvr_mat8_t *bvr_filter_create_grayscale(const bvr_io_image_source_t *src)
{
    size_t y, x, pixel_stride = 4;
    bvr_mat8_t *mat = (bvr_mat8_t *)malloc(sizeof(bvr_mat8_t));
    mat->rows = src->pixel_height;
    mat->columns = src->pixel_width;
    mat->content = (uint8_t *)calloc(src->data_len, sizeof(uint8_t));

    size_t bytes_in_row = src->pixel_width * pixel_stride;

    for (y = 0; y < src->pixel_height; y++)
    {
        for (x = 0; x < src->pixel_width; x++)
        {
            const uint8_t *pixel = src->data + x * pixel_stride + y * bytes_in_row;
            float r = pixel[0] / 255.0;
            float g = pixel[1] / 255.0;
            float b = pixel[2] / 255.0;

            // TODO: Check the method on https://en.wikipedia.org/wiki/Grayscale
            uint8_t val = (uint8_t)((0.299 * r + 0.587 * g + 0.114 * b) * 255.0);
            bvr_mat_set(mat, x, y, val);
        }
    }
    return mat;
}

bvr_matf64_t *bvr_filter_create_image_fintegral(const bvr_matf64_t *src)
{

    bvr_matf64_t *dst = bvr_matf64_new(src->columns, src->rows);
    size_t rows = src->rows;
    size_t columns = src->columns;

    dst->content[0] = src->content[0];

    // First row
    int i, y, x;
    for (i = 1; i < columns; i++)
    {
        uint64_t val = bvr_mat_get(src, i, 0) + bvr_mat_get(dst, i - 1, 0);
        bvr_mat_set(dst, i, 0, val);
    }

    // First column
    for (i = 1; i < rows; i++)
    {
        uint64_t val = bvr_mat_get(src, 0, i) + bvr_mat_get(dst, 0, i - 1);
        bvr_mat_set(dst, 0, i, val);
    }

    for (y = 1; y < rows; y++)
    {
        for (x = 1; x < columns; x++)
        {
            uint64_t val = bvr_mat_get(src, x, y) + bvr_mat_get(dst, x, y - 1) + bvr_mat_get(dst, x - 1, y) - bvr_mat_get(dst, x - 1, y - 1);
            bvr_mat_set(dst, x, y, val);
        }
    }

    return dst;
}
bvr_matf64_t *bvr_filter_create_image_integral(const bvr_mat8_t *src)
{

    bvr_matf64_t *dst = bvr_matf64_new(src->columns, src->rows);
    size_t rows = src->rows;
    size_t columns = src->columns;

    dst->content[0] = src->content[0];

    // First row
    int i, y, x;
    for (i = 1; i < columns; i++)
    {
        uint64_t val = bvr_mat_get(src, i, 0) + bvr_mat_get(dst, i - 1, 0);
        bvr_mat_set(dst, i, 0, val);
    }

    // First column
    for (i = 1; i < rows; i++)
    {
        uint64_t val = bvr_mat_get(src, 0, i) + bvr_mat_get(dst, 0, i - 1);
        bvr_mat_set(dst, 0, i, val);
    }

    for (y = 1; y < rows; y++)
    {
        for (x = 1; x < columns; x++)
        {
            uint64_t val = bvr_mat_get(src, x, y) + bvr_mat_get(dst, x, y - 1) + bvr_mat_get(dst, x - 1, y) - bvr_mat_get(dst, x - 1, y - 1);
            bvr_mat_set(dst, x, y, val);
        }
    }

    return dst;
}

int __bvr_bounded_x(const bvr_matf64_t *m, int w)
{

    if (w < 0)
        w = 0;
    if (w >= m->columns)
        w = m->columns - 1;
    return w;
}

int __bvr_bounded_y(const bvr_matf64_t *m, int h)
{

    if (h < 0)
        h = 0;
    if (h >= m->rows)
        h = m->rows - 1;
    return h;
}

double __bvr_mat_bounded_get(const bvr_matf64_t *m, int w, int h)
{
    return bvr_mat_get(m, __bvr_bounded_x(m, w), __bvr_bounded_y(m, h));
}

double __bvr_sum_w(const bvr_matf64_t *ii, size_t w_h, int x, int y)
{
    double a = __bvr_mat_bounded_get(ii, x - w_h, y - w_h);
    double b = __bvr_mat_bounded_get(ii, x + w_h - 1, y - w_h);
    double c = __bvr_mat_bounded_get(ii, x - w_h, y + w_h - 1);
    double d = __bvr_mat_bounded_get(ii, x + w_h - 1, y + w_h - 1);
    double sum = a + d - b - c;
    return sum;
}

bvr_mat8_t *bvr_filter_sauvola(const bvr_mat8_t *src, const double k, const size_t w, uint8_t foreground, uint8_t background)
{
    // Integral image gives the sum
    bvr_matf64_t *ii = bvr_filter_create_image_integral(src);
    bvr_matf64_t *s1 = ii;
    bvr_matf64_t *src2 = bvr_matf64_new(src->columns, src->rows);
    bvr_mat8_t *out = bvr_mat8_new(src->columns, src->rows);

    int w_h = round(w / 2);
    int y, x;

    for (y = 0; y < src->rows; y++)
    {
        for (x = 0; x < src->columns; x++)
        {
            double val = bvr_mat_get(src, x, y);
            bvr_mat_set(src2, x, y, val * val);
        }
    }

    bvr_matf64_t *s2 = bvr_filter_create_image_fintegral(src2);

    for (y = 0; y < ii->rows; y++)
    {
        for (x = 0; x < ii->columns; x++)
        {
            double n = (double)w * (double)w;
            double s1_ = __bvr_sum_w(s1, w_h, x, y);
            double s2_ = __bvr_sum_w(s2, w_h, x, y);

            double variance = 1 / n * (s2_ - s1_ * s1_ / n);
            double mean = s1_ / n;

            double stddev = sqrt(variance);
            double threshold = mean * (1 + k * ((stddev / 128.0) - 1));

            double current = (double)bvr_mat_get(src, x, y);
            if (current <= threshold)
            {
                bvr_mat_set(out, x, y, foreground); //foreground
            }
            else
            {
                bvr_mat_set(out, x, y, background); // Background
            }
        }
    }

    bvr_mat_free(ii);
    bvr_mat_free(src2);
    bvr_mat_free(s2);
    return out;
}
