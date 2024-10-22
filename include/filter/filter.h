#ifndef __BVRCORE_FILTER_H
#define __BVRCORE_FILTER_H

#include "matrix/matrix.h"
/**
 * @brief Compute a grayscale matrix from an image
 * 
 * The algorithm used is the one presented here: https://en.wikipedia.org/wiki/Grayscale
 * @param src The source image
 * * @param src The pixel stride (number of bytes to code the pixel)
 * @return bvr_mat8_t* The corresponding matrix containing the grayscale values
 */
bvr_mat8_t *bvr_filter_create_grayscale(const bvr_mat8_t *src, const size_t pixel_stride);

/**
 * @brief Create an image integer with integer values
 * 
 * @param src The grayscale input image
 * @return bvr_mat_real_t* The corresponding image integral matrix.
 */
bvr_mat_real_t *bvr_filter_create_image_integral(const bvr_mat8_t *src);

/**
 * @brief Create an image integer with floating values
 * 
 * @param src The grayscale input image
 * @return bvr_mat_real_t* The corresponding image integral matrix.
 */

bvr_mat_real_t *bvr_filter_create_image_fintegral(const bvr_mat_real_t *src);

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

/**
 * @brief Resize the matrix as image
 * 
 * @param src The source image as a gray matrix
 * @param width The new width
 * @param height The new height
 * @return bvr_mat8_t* 
 */
bvr_mat8_t *bvr_resize(const bvr_mat8_t *src, size_t width, size_t height);

#endif
