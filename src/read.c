#include "../include/read.h"
#include "../include/config.h"

// Mapping table: RGBA values mapped to alphabet letters
const PixelMap pixelMap[] = {
    { RGBA_A, 'A' },   // RGBA_A
    { RGBA_B, 'B' },   // RGBA_B
    { RGBA_C, 'C' },   // RGBA_C
    { RGBA_D, 'D' },   // RGBA_D
    { RGBA_E, 'E' },   // RGBA_E
    { RGBA_F, 'F' },   // RGBA_F
    { RGBA_G, 'G' },   // RGBA_G
    { RGBA_H, 'H' },   // RGBA_H
    { RGBA_I, 'I' },   // RGBA_I
    { RGBA_J, 'J' },   // RGBA_J
    { RGBA_K, 'K' },   // RGBA_K
    { RGBA_L, 'L' },   // RGBA_L
    { RGBA_M, 'M' },   // RGBA_M
    { RGBA_N, 'N' },   // RGBA_N
    { RGBA_O, 'O' },   // RGBA_O
    { RGBA_P, 'P' },   // RGBA_P
    { RGBA_Q, 'Q' },   // RGBA_Q
    { RGBA_R, 'R' },   // RGBA_R
    { RGBA_S, 'S' },   // RGBA_S
    { RGBA_T, 'T' },   // RGBA_T
    { RGBA_U, 'U' },   // RGBA_U
    { RGBA_V, 'V' },   // RGBA_V
    { RGBA_W, 'W' },   // RGBA_W
    { RGBA_X, 'X' },   // RGBA_X
    { RGBA_Y, 'Y' },   // RGBA_Y
    { RGBA_Z, 'Z' },   // RGBA_Z
    { RGBA_SPACE, ' ' },   // RGBA_SPACE
};


int ImageExist(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    // Buffer to hold the first few bytes of the file
    unsigned char buffer[8];
    size_t bytesRead = fread(buffer, 1, sizeof(buffer), fp);
    fclose(fp);

    if (bytesRead < 8) {
        return 0;
    }

    // Check for PNG magic number (89 50 4E 47 0D 0A 1A 0A)
    if (memcmp(buffer, "\x89PNG\r\n\x1A\n", 8) == 0) {
        return 1;
    }

    // Add checks for other image formats as needed

    return 0;
}

int comparePixelRGBA(const PixelRGBA *p1, const PixelRGBA *p2) {
    return p1->r == p2->r &&
           p1->g == p2->g &&
           p1->b == p2->b &&
           p1->a == p2->a;
}


PixelRGBA getPixelRGBA(const char *filename, int x, int y) {
    PixelRGBA pixel = {0, 0, 0, 0}; // Initialize to 0 (or any default values)

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File opening failed");
        return pixel;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        fprintf(stderr, "png_create_read_struct failed\n");
        return pixel;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        fprintf(stderr, "png_create_info_struct failed\n");
        return pixel;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        fprintf(stderr, "Error during init_io\n");
        return pixel;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int i = 0; i < height; i++) {
        row_pointers[i] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    if (x < width && y < height) {
        png_bytep px = &(row_pointers[y][x * 4]); // Assuming RGBA
        pixel.r = px[0];
        pixel.g = px[1];
        pixel.b = px[2];
        pixel.a = px[3];
    } else {
        fprintf(stderr, "Pixel coordinates out of bounds\n");
    }

    for (int i = 0; i < height; i++) {
        free(row_pointers[i]);
    }
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    return pixel;
}


int IsValid(const char *filename) {
  PixelRGBA x = getPixelRGBA(filename, 38, 39);
  if(x.a == 255) {
    return 1;
  }
  return 0;
}


char *generateEncodedAlphabet(const char *filename) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File opening failed");
        return NULL;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png) {
        fclose(fp);
        fprintf(stderr, "png_create_read_struct failed\n");
        return NULL;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_read_struct(&png, NULL, NULL);
        fclose(fp);
        fprintf(stderr, "png_create_info_struct failed\n");
        return NULL;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_read_struct(&png, &info, NULL);
        fclose(fp);
        fprintf(stderr, "Error during init_io\n");
        return NULL;
    }

    png_init_io(png, fp);
    png_read_info(png, info);

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    // Read any color_type into 8bit depth, RGBA format
    if (bit_depth == 16)
        png_set_strip_16(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE)
        png_set_palette_to_rgb(png);
    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
        png_set_expand_gray_1_2_4_to_8(png);
    if (png_get_valid(png, info, PNG_INFO_tRNS))
        png_set_tRNS_to_alpha(png);
    if (color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY)
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
        png_set_gray_to_rgb(png);

    png_read_update_info(png, info);

    png_bytep *row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
    for (int i = 0; i < height; i++) {
        row_pointers[i] = (png_byte*)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    size_t encoded_size = (width / 5) * (height / 1); // Assuming 5 pixels per letter, 1 row per letter
    char *encoded_string = (char *)malloc(encoded_size + 1); // +1 for null terminator
    encoded_string[0] = '\0'; // Initialize as empty string

    for (int y = 0; y < height; y += 1) { // Increment y by 1 for each letter
        for (int x = 0; x < width; x += 5) { // Increment x by 5 for each letter
            if (x + 5 <= width) {
                // Read RGBA values of the first pixel of the current letter
                png_bytep px = &(row_pointers[y][x * 4]); // Assuming RGBA format
                PixelRGBA pixel = {px[0], px[1], px[2], px[3]};

                // Search for the pixel in the pixelMap
                int found = 0;
                for (size_t i = 0; i < sizeof(pixelMap) / sizeof(pixelMap[0]); ++i) {
                    if (memcmp(&pixel, &pixelMap[i].rgba, sizeof(PixelRGBA)) == 0) {
                        // Found a match, append corresponding letter to encoded_string
                        strcat(encoded_string, &pixelMap[i].letter);
                        found = 1;
                        break;
                    }
                }

                if (!found) {
                  // TODO
                }
            }
        }
    }

    // Append termination sequence to the encoded string
    strcat(encoded_string, "#");

    // Free allocated memory
    for (int i = 0; i < height; i++) {
        free(row_pointers[i]);
    }
    free(row_pointers);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    return encoded_string;
}

