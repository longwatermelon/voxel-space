#include "prog.h"
#include "img.h"


struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->running = true;

    p->win = w;
    p->rend = r;

    p->color = image_alloc("color.png");
    p->height = image_alloc("height.png");

    return p;
}


void prog_free(struct Prog *p)
{
    image_free(p->height);
    image_free(p->color);
    free(p);
}


void prog_mainloop(struct Prog *p)
{
    SDL_Event evt;

    while (p->running)
    {
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                p->running = false;
                break;
            }
        }

        SDL_RenderClear(p->rend);

        prog_render_terrain(p);

        SDL_SetRenderDrawColor(p->rend, 0, 0, 0, 255);
        SDL_RenderPresent(p->rend);
    }
}


void prog_render_terrain(struct Prog *p)
{
    for (int y = 0; y < 400; ++y)
    {
        for (int x = 0; x < 400; ++x)
        {
            SDL_Color col = image_at(p->color, (float)x / 400 * p->color->w, (float)y / 400 * p->color->h);
            SDL_Color height = image_at(p->height, (float)x / 400 * p->height->w, (float)y / 400 * p->height->h);

            SDL_SetRenderDrawColor(p->rend, col.r, col.g, col.b, 255);
            SDL_RenderDrawPoint(p->rend, x, y);

            SDL_SetRenderDrawColor(p->rend, height.r, height.g, height.b, 255);
            SDL_RenderDrawPoint(p->rend, x + 400, y);
        }
    }
}

