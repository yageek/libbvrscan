#include "filter/filter.h"
#include "matrix/matrix.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

// Code used from
bvr_mat8_t *bvr_filter_create_grayscale(const bvr_mat8_t *src)
{
    size_t pixel_stride = 4;
    size_t pixel_width = src->columns / pixel_stride;
    size_t pixel_height = src->rows;
    size_t pixel_length = pixel_width * pixel_height;

    bvr_mat8_t *mat = (bvr_mat8_t *)malloc(sizeof(bvr_mat8_t));
    mat->rows = pixel_height;
    mat->columns = pixel_width;
    mat->content = (uint8_t *)calloc(pixel_length, sizeof(uint8_t));

    size_t px;
    for (px = 0; px < pixel_length; px++)
    {
        const uint8_t *pixel = &src->content[0] + px * pixel_stride;
        float r = pixel[0] / 255.0;
        float g = pixel[1] / 255.0;
        float b = pixel[2] / 255.0;

        // TODO: Check the method on https://en.wikipedia.org/wiki/Grayscale
        uint8_t val = (uint8_t)((0.299 * r + 0.587 * g + 0.114 * b) * 255.0);
        mat->content[px] = val;
    }
    return mat;
}

bvr_mat_real_t *bvr_filter_create_image_fintegral(const bvr_mat_real_t *src)
{

    bvr_mat_real_t *dst = bvr_mat_real_new(src->rows, src->columns);
    size_t rows = src->rows;
    size_t columns = src->columns;

    dst->content[0] = src->content[0];

    // First row
    int i, y, x;
    for (i = 1; i < columns; i++)
    {
        uint64_t val = bvr_mat_get(src, 0, i) + bvr_mat_get(dst, 0, i - 1);
        bvr_mat_set(dst, 0, i, val);
    }

    // First column
    for (i = 1; i < rows; i++)
    {
        uint64_t val = bvr_mat_get(src, i, 0) + bvr_mat_get(dst, i - 1, 0);
        bvr_mat_set(dst, i, 0, val);
    }

    for (y = 1; y < rows; y++)
    {
        for (x = 1; x < columns; x++)
        {
            uint64_t val = bvr_mat_get(src, y, x) + bvr_mat_get(dst, y - 1, x) + bvr_mat_get(dst, y, x - 1) - bvr_mat_get(dst, y - 1, x - 1);
            bvr_mat_set(dst, y, x, val);
        }
    }

    return dst;
}
bvr_mat_real_t *bvr_filter_create_image_integral(const bvr_mat8_t *src)
{

    bvr_mat_real_t *dst = bvr_mat_real_new(src->rows, src->columns);
    size_t rows = src->rows;
    size_t columns = src->columns;

    dst->content[0] = src->content[0];

    // First row
    int i, y, x;
    for (i = 1; i < columns; i++)
    {
        uint64_t val = bvr_mat_get(src, 0, i) + bvr_mat_get(dst, 0, i - 1);
        bvr_mat_set(dst, 0, i, val);
    }

    // First column
    for (i = 1; i < rows; i++)
    {
        uint64_t val = bvr_mat_get(src, i, 0) + bvr_mat_get(dst, i - 1, 0);
        bvr_mat_set(dst, i, 0, val);
    }

    for (y = 1; y < rows; y++)
    {
        for (x = 1; x < columns; x++)
        {
            uint64_t val = bvr_mat_get(src, y, x) + bvr_mat_get(dst, y - 1, x) + bvr_mat_get(dst, y, x - 1) - bvr_mat_get(dst, y - 1, x - 1);
            bvr_mat_set(dst, y, x, val);
        }
    }

    return dst;
}

int __bvr_bounded_column(const bvr_mat_real_t *m, int col)
{

    if (col < 0)
        col = 0;
    if (col >= m->columns)
        col = m->columns - 1;
    return col;
}

int __bvr_bounded_row(const bvr_mat_real_t *m, int row)
{

    if (row < 0)
        row = 0;
    if (row >= m->rows)
        row = m->rows - 1;
    return row;
}

double __bvr_mat_bounded_get(const bvr_mat_real_t *m, int row, int col)
{
    return bvr_mat_get(m, __bvr_bounded_row(m, row), __bvr_bounded_column(m, col));
}

double __bvr_sum_w(const bvr_mat_real_t *ii, size_t w_h, int row, int col)
{
    double a = __bvr_mat_bounded_get(ii, row - w_h, col - w_h);
    double b = __bvr_mat_bounded_get(ii, row - w_h, col + w_h - 1);
    double c = __bvr_mat_bounded_get(ii, row + w_h - 1, col - w_h);
    double d = __bvr_mat_bounded_get(ii, row + w_h - 1, col + w_h - 1);
    double sum = a + d - b - c;
    return sum;
}

bvr_mat8_t *bvr_filter_sauvola(const bvr_mat8_t *src, const double k, const size_t w, uint8_t foreground, uint8_t background)
{
    // Integral image gives the sum
    bvr_mat_real_t *ii = bvr_filter_create_image_integral(src);
    bvr_mat_real_t *s1 = ii;
    bvr_mat_real_t *src2 = bvr_mat_real_new(src->rows, src->columns);
    bvr_mat8_t *out = bvr_mat8_new(src->rows, src->columns);

    int w_h = round(w / 2);
    int row, col;

    for (row = 0; row < src->rows; row++)
    {
        for (col = 0; col < src->columns; col++)
        {
            double val = bvr_mat_get(src, row, col);
            bvr_mat_set(src2, row, col, val * val);
        }
    }

    bvr_mat_real_t *s2 = bvr_filter_create_image_fintegral(src2);

    for (row = 0; row < ii->rows; row++)
    {
        for (col = 0; col < ii->columns; col++)
        {
            double n = (double)w * (double)w;
            double s1_ = __bvr_sum_w(s1, w_h, row, col);
            double s2_ = __bvr_sum_w(s2, w_h, row, col);

            double variance = 1 / n * (s2_ - s1_ * s1_ / n);
            double mean = s1_ / n;

            double stddev = sqrt(variance);
            double threshold = mean * (1 + k * ((stddev / 128.0) - 1));

            double current = (double)bvr_mat_get(src, row, col);
            if (current <= threshold)
            {
                bvr_mat_set(out, row, col, foreground); //foreground
            }
            else
            {
                bvr_mat_set(out, row, col, background); // Background
            }
        }
    }

    bvr_mat_free(ii);
    bvr_mat_free(src2);
    bvr_mat_free(s2);
    return out;
}

// See: http://www.tech-algorithm.com/articles/bilinear-image-scaling/
bvr_mat8_t *bvr_resize(const bvr_mat8_t *src, size_t width, size_t height)
{
    // public int[] resizeBilinearGray(int[] pixels, int w, int h, int w2, int h2) {

    // int[] temp = new int[w2*h2] ;

    int w = src->columns;
    int h = src->rows;
    int h2 = height;
    int w2 = width;

    bvr_mat8_t *temp = bvr_mat8_new(h2, w2);
    int A, B, C, D, x, y, index, gray;
    float x_ratio = ((float)(w - 1)) / w2;
    float y_ratio = ((float)(h - 1)) / h2;
    float x_diff, y_diff, ya, yb;
    int offset = 0;
    for (int i = 0; i < h2; i++)
    {
        for (int j = 0; j < w2; j++)
        {
            x = (int)(x_ratio * j);
            y = (int)(y_ratio * i);
            x_diff = (x_ratio * j) - x;
            y_diff = (y_ratio * i) - y;
            index = y * w + x;

            // range is 0 to 255 thus bitwise AND with 0xff
            A = src->content[index] & 0xff;
            B = src->content[index + 1] & 0xff;
            C = src->content[index + w] & 0xff;
            D = src->content[index + w + 1] & 0xff;

            // Y = A(1-w)(1-h) + B(w)(1-h) + C(h)(1-w) + Dwh
            gray = (int)(A * (1 - x_diff) * (1 - y_diff) + B * (x_diff) * (1 - y_diff) +
                         C * (y_diff) * (1 - x_diff) + D * (x_diff * y_diff));

            temp->content[offset++] = gray;
        }
    }
    return temp;
}
