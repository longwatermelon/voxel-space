#ifndef PROG_H
#define PROG_H

#include <stdbool.h>
#include <SDL2/SDL.h>

struct Prog
{
    bool running;

    SDL_Window *win;
    SDL_Renderer *rend;

    struct Image *color, *height;
};

struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r);
void prog_free(struct Prog *p);

void prog_mainloop(struct Prog *p);

void prog_render_terrain(struct Prog *p);

#endif

