#include "prog.h"


int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *w = SDL_CreateWindow("Voxel space", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    struct Prog *p = prog_alloc(w, r);
    prog_mainloop(p);
    prog_free(p);

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);

    SDL_Quit();

    return 0;
}

