#ifndef CHANGE
#define CHANGE

#include <stdlib.h>
#include <stdio.h>
#include <png.h>

#include "read.h"

void change_pixels(const char *input_filename, const char *output_filename, PixelRGBA *pixels, int num_pixels);


#endif 


