#include "../include/change.h"
#include <stdio.h>

void change_pixels(const char *input_filename, const char *output_filename, PixelRGBA *pixels, int num_pixels) {
    FILE *fp_in = fopen(input_filename, "rb");
    if (!fp_in) {
        printf("%s\n", input_filename);
        perror("File opening failed");
        return;
    }

    FILE *fp_out = fopen(output_filename, "wb");
    if (!fp_out) {
        perror("File opening failed");
        fclose(fp_in);
        return;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp_in);
        fclose(fp_out);
        fprintf(stderr, "png_create_read_struct failed\n");
        return;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp_in);
        fclose(fp_out);
        fprintf(stderr, "png_create_info_struct failed\n");
        return;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp_in);
        fclose(fp_out);
        fprintf(stderr, "Error during init_io\n");
        return;
    }

    png_init_io(png, fp_in);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16) {
        png_set_strip_16(png);
    }
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) {
        png_set_expand_gray_1_2_4_to_8(png);
    }
    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
    }
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY) {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    if (38 < width && 39 < height) {
        png_bytep px = &(row_pointers[39][38 * 4]); // Assuming RGBA format
        px[3] = 255;    
    }

    int current_x = 10; // Starting x position
    int current_y = 10; // Starting y position

    for (int i = 0; i < num_pixels; i++) {
        if (current_y >= 0 && current_y < height) {
            if (current_x >= 0 && current_x < width) {
                png_bytep px = &(row_pointers[current_y][current_x * 4]); // Assuming RGBA format
                px[0] = (png_byte)pixels[i].r;    // Red
                px[1] = (png_byte)pixels[i].g;    // Green
                px[2] = (png_byte)pixels[i].b;    // Blue
                px[3] = (png_byte)pixels[i].a;    // Alpha
            } else {
                fprintf(stderr, "Pixel coordinate (%d, %d) out of bounds\n", current_x, current_y);
            }
        } else {
            fprintf(stderr, "Pixel coordinate (%d, %d) out of bounds\n", current_x, current_y);
        }
        current_x += 5; // Increment x position
    }

    png_structp png_out = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_out) {
        fclose(fp_in);
        fclose(fp_out);
        fprintf(stderr, "png_create_write_struct failed\n");
        return;
    }

    png_infop info_out = png_create_info_struct(png_out);
    if (!info_out) {
        png_destroy_write_struct(&png_out, NULL);
        fclose(fp_in);
        fclose(fp_out);
        fprintf(stderr, "png_create_info_struct failed\n");
        return;
    }

    if (setjmp(png_jmpbuf(png_out))) {
        png_destroy_write_struct(&png_out, &info_out);
        fclose(fp_in);
        fclose(fp_out);
        fprintf(stderr, "Error during init_io (output)\n");
        return;
    }

    png_init_io(png_out, fp_out);

    png_set_IHDR(
        png_out,
        info_out,
        width, height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    );

    png_write_info(png_out, info_out);
    png_write_image(png_out, row_pointers);
    png_write_end(png_out, NULL);

    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
    png_destroy_write_struct(&png_out, &info_out);
    fclose(fp_in);
    fclose(fp_out);
}
