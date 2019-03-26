#include "io/io.h"

#include <CoreFoundation/CoreFoundation.h>
#include <ImageIO/ImageIO.h>
#include <CoreServices/CoreServices.h>

int bvr_io_load_image_generic(const char *path_name, bvr_io_image_source_t *dst, CGColorSpaceModel colorModel, int imageBitsPerPixel, int imageBitsPerComponent)
{
    CFStringRef path = CFStringCreateWithCString(NULL, path_name, kCFStringEncodingASCII);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, 0);

    CGImageSourceRef img_src = CGImageSourceCreateWithURL(url, NULL);

    if (img_src == NULL)
    {
        fprintf(stderr, "Image source is NULL.");
        CFRelease(url);
        CFRelease(path);
        return -1;
    }

    CFRelease(url);
    CFRelease(path);

    CGImageRef img = CGImageSourceCreateImageAtIndex(img_src, 0, NULL);

    if (img == NULL)
    {
        return -1;
    }

    // Check that the image is in the expected format
    CGColorSpaceRef colorSpace = CGImageGetColorSpace(img);

    if (colorSpace == NULL)
    {
        return -1;
    }

    CGColorSpaceModel model = CGColorSpaceGetModel(colorSpace);
    if (model != colorModel)
    {
        return -1;
    }
    size_t bitsPerPixel = CGImageGetBitsPerPixel(img);
    size_t bitsPerComponent = CGImageGetBitsPerComponent(img);

    if (bitsPerPixel != imageBitsPerPixel && bitsPerComponent != imageBitsPerComponent)
    {
        return -1;
    }

    // Loading the different informations from the image
    dst->pixel_width = CGImageGetWidth(img);
    dst->pixel_height = CGImageGetHeight(img);

    CFDataRef data = CGDataProviderCopyData(CGImageGetDataProvider(img));
    CFIndex len = CFDataGetLength(data);
    dst->data = malloc(len);
    CFDataGetBytes(data, CFRangeMake(0, len), dst->data);
    dst->data_len = len;

    // Releasing memory
    CFRelease(img_src);
    CFRelease(img);
    CFRelease(data);

    return 0;
}

int bvr_io_load_image_rgb(const char *path_name, bvr_io_image_source_t *dst)
{
    return bvr_io_load_image_generic(path_name, dst, kCGColorSpaceModelRGB, 32, 8);
}

int bvr_io_load_image_grayscale(const char *path_name, bvr_io_image_source_t *dst)
{
    return bvr_io_load_image_generic(path_name, dst, kCGColorSpaceModelMonochrome, 8, 8);
}
void bvr_io_image_source_free(bvr_io_image_source_t dst)
{
    free(dst.data);
    dst.data_len = 0;
    dst.pixel_height = 0;
    dst.pixel_width = 0;
}

int bvr_io_image_grayscale_write(const bvr_mat8_t *src, const char *path_name, const BVRWritingType type)
{

    // We first create the image
    CGColorSpaceRef colorspace = CGColorSpaceCreateDeviceGray();
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, src->content, src->columns * src->rows, NULL);
    CGImageRef img = CGImageCreate(src->columns, src->rows, 8, 8, src->columns, colorspace, kCGBitmapByteOrderDefault, provider, NULL, 0, kCGRenderingIntentDefault);
    CGDataProviderRelease(provider);
    CGColorSpaceRelease(colorspace);

    if (img == NULL)
    {
        return -1;
    }

    // Create the imag destination
    CFStringRef path = CFStringCreateWithCString(NULL, path_name, kCFStringEncodingASCII);
    CFURLRef url = CFURLCreateWithFileSystemPath(NULL, path, kCFURLPOSIXPathStyle, 0);

    CFStringRef refType = type == BVRWritingTypeJPG ? kUTTypeJPEG : kUTTypePNG;
    CGImageDestinationRef dest = CGImageDestinationCreateWithURL(url, refType, 1, NULL);
    CFRelease(url);
    CFRelease(path);
    if (dest == NULL)
    {
        return -1;
    }

    CGImageDestinationAddImage(dest, img, NULL);

    // Finish
    bool result = CGImageDestinationFinalize(dest);
    CGImageRelease(img);

    CFRelease(dest);
    return result ? 0 : -1;
}
