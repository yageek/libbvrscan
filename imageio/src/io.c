#include "io.h"

#include <cairo/cairo.h>
#include <string.h>

struct _bvr_io_image_source_t
{
    cairo_surface_t *cairo_surface;
    size_t pixel_stride;
};

bvr_io_image_source_t *bvr_io_load_png(const char *path_name)
{
    // Read PNG
    cairo_surface_t *surface = cairo_image_surface_create_from_png(path_name);
    cairo_status_t stat = cairo_surface_status(surface);
    if (stat != CAIRO_STATUS_SUCCESS)
    {
        return NULL;
    }

    // Check known formats
    size_t pixel_stride;
    int format = cairo_image_surface_get_format(surface);

    switch (format)
    {
    case CAIRO_FORMAT_ARGB32:
        pixel_stride = 4;
        break;
    case CAIRO_FORMAT_RGB24:
        pixel_stride = 3;
        break;
    default:
        return NULL;
    }

    struct _bvr_io_image_source_t *src = (struct _bvr_io_image_source_t *)malloc(sizeof(struct _bvr_io_image_source_t));
    src->cairo_surface = surface;
    src->pixel_stride = pixel_stride;
    return src;
}
int bvr_io_image_source_pixel_stride(const bvr_io_image_source_t *src)
{
    return src->pixel_stride;
}
void bvr_io_image_source_free(bvr_io_image_source_t *dst)
{
    cairo_surface_destroy(dst->cairo_surface);
    free(dst);
}

int bvr_io_image_grayscale_write(const bvr_mat8_t *src, const char *path_name)
{
    int stride = cairo_format_stride_for_width(CAIRO_FORMAT_A8, src->columns);
    uint8_t *data = malloc(stride * src->rows);
    size_t row;
    for (row = 0; row < src->rows; row++)
    {
        memcpy(&data[0] + row * stride, &src->content[0] + row * src->columns, src->columns);
    }

    cairo_surface_t *surface = cairo_image_surface_create_for_data(data, CAIRO_FORMAT_A8, src->columns, src->rows, stride);
    int res = cairo_surface_write_to_png(surface, path_name);

    cairo_surface_destroy(surface);
    return res;
}

bvr_mat8_t *bvr_io_image_src(const bvr_io_image_source_t *src)
{
    int format = cairo_image_surface_get_format(src->cairo_surface);
    int stride = cairo_image_surface_get_stride(src->cairo_surface);
    int height = cairo_image_surface_get_height(src->cairo_surface);
    uint8_t *data = cairo_image_surface_get_data(src->cairo_surface);
    bvr_mat8_t *dst = bvr_mat8_new(height, stride);
    memcpy(&dst->content[0], data, height * stride);
    return dst;
}
