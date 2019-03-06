#ifndef __BVRCORE_FILTER_H
#define __BVRCORE_FILTER_H

#include "matrix/matrix.h"
#include "io/io.h"
/**
 * @brief Compute a grayscale matrix from an image
 * 
 * The algorithm used is the one presented here: https://en.wikipedia.org/wiki/Grayscale
 * @param src The source image
 * @return bvr_mat8_t* The corresponding matrix containing the grayscale values
 */
bvr_mat8_t *bvr_filter_create_grayscale(const bvr_io_image_source_t *src);

/**
 * @brief Create an image integer with integer values
 * 
 * @param src The grayscale input image
 * @return bvr_matf32_t* The corresponding image integral matrix.
 */
bvr_matf32_t *bvr_filter_create_image_integral(const bvr_mat8_t *src);

/**
 * @brief Create an image integer with floating values
 * 
 * @param src The grayscale input image
 * @return bvr_matf32_t* The corresponding image integral matrix.
 */

bvr_matf32_t *bvr_filter_create_image_fintegral(const bvr_matf32_t *src);

/**
 * @brief Filter the provided grayscale image with the Sauvola algorithm.
 * 
 * @param src The input grayscale image
 * @param k The k parameter of the filter
 * @param window_size The size of the local windows
 * @param foregound The value used to represent foreground in the result matrix
 * @param background The value used to represent background in the result matrix
 * @return bvr_mat8_t* The filtered image
 */
bvr_mat8_t *bvr_filter_sauvola(const bvr_mat8_t *src, const double k, const size_t window_size, uint8_t foregound, uint8_t background);
#endif
