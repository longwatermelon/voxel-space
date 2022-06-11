#ifndef IMG_H
#define IMG_H

#include <png.h>
#include <SDL2/SDL.h>

struct Image
{
    int w, h;
    png_bytep *rows;

    png_structp ptr;
    png_infop info;
};

struct Image *image_alloc(const char *src);
void image_free(struct Image *img);

SDL_Color image_at(struct Image *img, int x, int y);

#endif

