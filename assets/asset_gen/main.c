//
//  main.m
//  asset_gen
//
//  Created by Yannick Heinrich on 11.10.18.
//  Copyright © 2018 yageek. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <cairo/cairo.h>
#include <unistd.h>

#include <sys/stat.h>
#include <ftw.h>

#define ASSET_WIDTH 32
#define ASSET_HEIGHT 32

// See: https://stackoverflow.com/questions/5467725/how-to-delete-a-directory-and-its-contents-in-posix-c
int unlink_cb(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    int rv = remove(fpath);

    if (rv)
        perror(fpath);

    return rv;
}

int rmrf(const char *path)
{
    return nftw(path, unlink_cb, 64, FTW_DEPTH | FTW_PHYS);
}

int create_dir_if_non_exists(const char *output)
{
    struct stat s;
    int err = stat(output, &s);

    // If directory exists
    if (err == 0 && S_ISDIR(s.st_mode))
    {
        err = rmrf(output);
        if (err < 0)
        {
            perror("can not delete exising directory");
            return err;
        }
    }

    if (err < 0 && errno != ENOENT)
    {
        perror("can not stat file");
        return err;
    }

    return mkdir(output, 0777);
}

int main(int argc, const char *argv[])
{

    const char *assets_list = "0123456789<+>";
    char output_buff[1024];

    // Check input arguments
    if (argc < 2)
    {
        printf("Name output at minimum\n");
        exit(-1);
    }

    // Create outputs dir
    char *output_path = realpath(argv[1], NULL);
    snprintf(output_buff, 1024, "%s/generated", output_path);

    if (create_dir_if_non_exists(output_buff) < 0)
    {
        exit(-1);
    }

    int i = 0;
    for (i = 0; i < strlen(assets_list); i++)
    {
        char current = assets_list[i];

        // Create the surface
        cairo_surface_t *surface;
        cairo_t *cr;

        surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ASSET_WIDTH, ASSET_HEIGHT);
        cr = cairo_create(surface);

        // Background
        cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
        cairo_paint(cr);

        // Draw the character
        cairo_text_extents_t te;

        cairo_select_font_face(cr, "OCRB",
                               CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
        cairo_set_font_size(cr, 30.0);
        cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1.0);

        cairo_text_extents(cr, &current, &te);

        double x = (ASSET_WIDTH - te.width) / 2.0 - te.x_bearing;
        double y = (ASSET_HEIGHT - te.height) / 2.0 - te.y_bearing;
        cairo_move_to(cr, x, y);
        cairo_show_text(cr, &current);

        // Save file to png
        snprintf(output_buff, 1024, "%s/generated/%c.png", output_path, current);
        cairo_status_t status = cairo_surface_write_to_png(cairo_get_target(cr), output_buff);
        if (status != CAIRO_STATUS_SUCCESS)
        {
            perror(cairo_status_to_string(status));
            exit(-1);
        }
    }

    free(output_path);
    return 0;
}
