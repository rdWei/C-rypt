#ifndef READ_H
#define READ_H

#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <string.h>

typedef struct {
    int r;
    int g;
    int b;
    int a;
} PixelRGBA;

typedef struct {
    PixelRGBA rgba;
    char letter;
} PixelMap;


int ImageExist(const char *filename);
int IsValid(const char *filename);
PixelRGBA getPixelRGBA(const char *filename, int x, int y);
char *generateEncodedAlphabet(const char *filename);
char* FormattedTime();
#endif
