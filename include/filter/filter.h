#ifndef __BVRCORE_FILTER_H
#define __BVRCORE_FILTER_H

#include "matrix/matrix.h"
#include "io/io.h"

bvr_mat8_t *bvr_filter_create_grayscale(const bvr_io_image_source_t *src);

bvr_matf32_t *bvr_filter_create_image_integral(const bvr_mat8_t *src);
bvr_matf32_t *bvr_filter_create_image_fintegral(const bvr_matf32_t *src);
bvr_mat8_t *bvr_filter_sauvola(const bvr_mat8_t *src, const double k, const size_t window_size, uint8_t foregound, uint8_t background);
#endif
