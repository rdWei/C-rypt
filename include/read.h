#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>

int ImageExist(const char *filename);
void readPngFile(const char *filename);

#endif
