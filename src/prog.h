#ifndef PROG_H
#define PROG_H

#include "camera.h"
#include "img.h"
#include <stdbool.h>
#include <SDL2/SDL.h>

struct Prog
{
    bool running;

    SDL_Window *win;
    SDL_Renderer *rend;

    struct Image *color, *height;

    struct Camera *cam;

    int ybuf[800];
};

struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r);
void prog_free(struct Prog *p);

void prog_mainloop(struct Prog *p);

void prog_render_terrain(struct Prog *p);
SDL_Point prog_image_coords(struct Prog *p, struct Image *img, float x, float y);

void prog_switch_map(struct Prog *p, const char *color, const char *height);

Vec2f prog_matmul(float mat[2][2], Vec2f p);

void prog_reset_ybuf(struct Prog *p);

#endif

