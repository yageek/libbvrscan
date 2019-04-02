#ifndef __BVRCORE_IMAGEIO
#define __BVRCORE_IMAGEIO

#include "matrix/matrix.h"

typedef struct _bvr_io_image_source_t bvr_io_image_source_t;

/**
 * @brief Load the image at the provided path as sRGB
 * **NOTE**: iOS https://developer.apple.com/documentation/uikit/uicolor?language=objc#1968626
 * 
 * @param path_name The path where to load the file
 * @param dst The pointer destination to update
 * @return int 
 */
bvr_io_image_source_t *bvr_io_load_png(const char *path_name);
bvr_mat8_t *bvr_io_grayscale_src(const bvr_io_image_source_t *src);
bvr_mat8_t *bvr_io_rgb_src(const bvr_io_image_source_t *src);
/**
 * @brief Release memory of an image source
 * 
 * @param dst The image source whose memory should be gree
 */
void bvr_io_image_source_free(bvr_io_image_source_t *dst);

/**
 * @brief Write the specified grayscale as output image as PNG
 * 
 * @param src The matrix reprensenting the grayscale image
 * @param path_name The path where to save the image
 * @return int The result of the operatiobn
 */
int bvr_io_image_grayscale_write(const bvr_mat8_t *src, const char *path_name);
#endif