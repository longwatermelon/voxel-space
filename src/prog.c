#include "prog.h"
#include "img.h"
#include <math.h>


struct Prog *prog_alloc(SDL_Window *w, SDL_Renderer *r)
{
    struct Prog *p = malloc(sizeof(struct Prog));
    p->running = true;

    p->win = w;
    p->rend = r;

    p->color = image_alloc("color.png");
    p->height = image_alloc("height.png");

    p->cam = cam_alloc((Vec2f){ 400, 400 }, 0.f);

    return p;
}


void prog_free(struct Prog *p)
{
    cam_free(p->cam);
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

        const Uint8 *keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_LEFT]) p->cam->angle -= .03f;
        if (keys[SDL_SCANCODE_RIGHT]) p->cam->angle += .03f;

        SDL_RenderClear(p->rend);

        prog_render_terrain(p);

        SDL_SetRenderDrawColor(p->rend, 0, 0, 0, 255);
        SDL_RenderPresent(p->rend);
    }
}


void prog_render_terrain(struct Prog *p)
{
#if 0
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
#endif

    /* for (int y = 0; y < 800; ++y) */
    /* { */
    /*     for (int x = 0; x < 800; ++x) */
    /*     { */
    /*         SDL_Color col = image_at(p->color, (float)x / 800 * p->color->w, (float)y / 800 * p->color->h); */
    /*         SDL_SetRenderDrawColor(p->rend, col.r, col.g, col.b, 255); */
    /*         SDL_RenderDrawPoint(p->rend, x, y); */
    /*     } */
    /* } */

    /* SDL_Rect cam = { p->cam->pos.x - 5, p->cam->pos.y - 5, 10, 10 }; */
    /* SDL_SetRenderDrawColor(p->rend, 255, 0, 0, 255); */
    /* SDL_RenderFillRect(p->rend, &cam); */

    float left = -M_PI / 4.f;
    float right = M_PI / 4.f;

    float rotl[2][2] = {
        { cosf(left), -sinf(left) },
        { sinf(left), cosf(left) }
    };

    float rotr[2][2] = {
        { cosf(right), -sinf(right) },
        { sinf(right), cosf(right) }
    };

    float rot[2][2] = {
        { cosf(p->cam->angle), -sinf(p->cam->angle) },
        { sinf(p->cam->angle), cosf(p->cam->angle) }
    };

    Vec2f dir = prog_matmul(rot, (Vec2f){ 1, 0 });

    for (float z = 300.f; z > 1.f; z -= 1)
    {
        Vec2f lp = vec_addv(p->cam->pos, prog_matmul(rotl, vec_mulf(dir, z)));
        Vec2f rp = vec_addv(p->cam->pos, prog_matmul(rotr, vec_mulf(dir, z)));

        float dx = (rp.x - lp.x) / 800.f;
        float dy = (rp.y - lp.y) / 800.f;

        for (int i = 0; i < 800; ++i)
        {
            float height = image_at(p->height, lp.x / 800.f * p->height->w, lp.y / 800.f * p->height->h).r;
            height /= z;
            height = 0 + (height + .5f) * 800.f;

            SDL_Color col = image_at(p->color, lp.x / 800.f * p->color->w, lp.y / 800.f * p->color->h);
            SDL_SetRenderDrawColor(p->rend, col.r, col.g, col.b, 255);
            SDL_RenderDrawLine(p->rend, i, height, i, 800);

            lp.x += dx;
            lp.y += dy;
        }
    }
}


Vec2f prog_matmul(float mat[2][2], Vec2f p)
{
    Vec2f ret;

    ret.x = mat[0][0] * p.x + mat[0][1] * p.y;
    ret.y = mat[1][0] * p.x + mat[1][1] * p.y;

    return ret;
}

