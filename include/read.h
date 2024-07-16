#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <png.h>

int ImageExsist(const char *filename);
void readPngFile(const char *filename);

#endif
