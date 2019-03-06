#ifndef __BVRCORE_IMAGEIO
#define __BVRCORE_IMAGEIO

#include "matrix/matrix.h"
typedef struct
{
    uint8_t *data;
    size_t pixel_width;
    size_t pixel_height;
    size_t data_len;

} bvr_io_image_source_t;

/**
 * @brief Load the image at the provided path as sRGB
 * **NOTE**: iOS https://developer.apple.com/documentation/uikit/uicolor?language=objc#1968626
 * 
 * @param path_name The path where to load the file
 * @param dst The pointer destination to update
 * @return int 
 */
int bvr_io_load_image(const char *path_name, bvr_io_image_source_t *dst);

/**
 * @brief Release memory of an image source
 * 
 * @param dst The image source whose memory should be gree
 */
void bvr_io_image_source_free(bvr_io_image_source_t dst);

/**
 * @brief An enum representing the output 
 * writing format of an image
 */
typedef enum
{
    BVRWritingTypePNG,
    BVRWritingTypeJPG
} BVRWritingType;

/**
 * @brief Write the specified grayscale as output image
 * 
 * @param src The matrix reprensenting the grayscale image
 * @param path_name The path where to save the image
 * @param type The type of image to save (PNG or JPG)
 * @return int The result of the operatiobn
 */
int bvr_io_image_grayscale_write(const bvr_mat8_t *src, const char *path_name, const BVRWritingType type);
#endif