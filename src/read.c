#include "../include/read.h"

int ImageExsist(const char *filename) {
  FILE *fp = fopen(filename, "rb");
  if(!fp) {
    return 0;
  }
  return 1;
}

void readPngFile(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if(!fp) {
        perror("File opening failed");
        return;
    }

    // Initialize read structure
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        fprintf(stderr, "png_create_read_struct failed\n");
        return;
    }

    // Initialize info structure
    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        fprintf(stderr, "png_create_info_struct failed\n");
        return;
    }

    // Error handling
    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        fprintf(stderr, "Error during init_io\n");
        return;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format
    if (bit_depth == 16) png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY) png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    // Allocate memory for the rows
    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    // Get pixel at (10, 10)
    if (width > 10 && height > 10) {
        png_bytep px = &(row_pointers[10][10 * 4]); // Assuming RGBA
        printf("Pixel at (10, 10): R=%d G=%d B=%d A=%d\n", px[0], px[1], px[2], px[3]);
    } else {
        fprintf(stderr, "Image is too small\n");
    }

    // Cleanup
    for (int y = 0; y < height; y++) {
        free(row_pointers[y]);
    }
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
}

/*int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    read_png_file(argv[1]);
    return EXIT_SUCCESS;
}*/

