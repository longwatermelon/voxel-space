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

    p->cam = cam_alloc((Vec2f){ 400, 400 }, 0.f, 0.f);

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
            case SDL_KEYDOWN:
            {
                switch (evt.key.keysym.sym)
                {
                case SDLK_1:
                    prog_switch_map(p, "color.png", "height.png");
                    break;
                case SDLK_2:
                    prog_switch_map(p, "color2.png", "height2.png");
                    break;
                case SDLK_3:
                    prog_switch_map(p, "color3.png", "height3.png");
                    break;
                }
            } break;
            }
        }

        const Uint8 *keys = SDL_GetKeyboardState(0);

        if (keys[SDL_SCANCODE_LEFT]) p->cam->angle -= .03f;
        if (keys[SDL_SCANCODE_RIGHT]) p->cam->angle += .03f;
        if (keys[SDL_SCANCODE_SPACE]) p->cam->height += 5.f;
        if (keys[SDL_SCANCODE_LSHIFT]) p->cam->height -= 5.f;

        if (keys[SDL_SCANCODE_W])
        {
            p->cam->pos.x += 5.f * cosf(p->cam->angle);
            p->cam->pos.y += 5.f * sinf(p->cam->angle);
        }

        prog_reset_heightbuf(p);

        SDL_Point coords = prog_image_coords(p, p->height, p->cam->pos.x, p->cam->pos.y);
        float height = (255.f - image_at(p->height, coords.x, coords.y).r);

        if (-p->cam->height > height - 10)
            p->cam->height = -(height - 10);

        SDL_RenderClear(p->rend);

        prog_render_terrain(p);

        SDL_SetRenderDrawColor(p->rend, 0, 0, 0, 255);
        SDL_RenderPresent(p->rend);
    }
}


void prog_render_terrain(struct Prog *p)
{
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
    float dz = 1.f;

    for (float z = 600.f; z > 1.f; z -= dz)
    {
        Vec2f lp = vec_addv(p->cam->pos, prog_matmul(rotl, vec_mulf(dir, z)));
        Vec2f rp = vec_addv(p->cam->pos, prog_matmul(rotr, vec_mulf(dir, z)));

        float dx = (rp.x - lp.x) / 800.f;
        float dy = (rp.y - lp.y) / 800.f;

        for (int i = 0; i < 800; ++i)
        {
            SDL_Point coords = prog_image_coords(p, p->height, lp.x, lp.y);
            float height = p->cam->height + (255.f - image_at(p->height, coords.x, coords.y).r);
            height /= z;
            height = (height + 1.f) * 400.f;

            int bottom = 800;

            if (height < p->heightbuf[i])
            {
                bottom = p->heightbuf[i];
                p->heightbuf[i] = height;
            }

            coords = prog_image_coords(p, p->color, lp.x, lp.y);
            SDL_Color col = image_at(p->color, coords.x, coords.y);
            SDL_SetRenderDrawColor(p->rend, col.r, col.g, col.b, 255);
            SDL_RenderDrawLine(p->rend, i, height, i, bottom);

            lp.x += dx;
            lp.y += dy;
        }

        dz += .01f;
    }
}


void prog_switch_map(struct Prog *p, const char *color, const char *height)
{
    image_free(p->color);
    image_free(p->height);

    p->color = image_alloc(color);
    p->height = image_alloc(height);
}


Vec2f prog_matmul(float mat[2][2], Vec2f p)
{
    Vec2f ret;

    ret.x = mat[0][0] * p.x + mat[0][1] * p.y;
    ret.y = mat[1][0] * p.x + mat[1][1] * p.y;

    return ret;
}


SDL_Point prog_image_coords(struct Prog *p, struct Image *img, float x, float y)
{
    if (x < 0) x *= -1;
    if (y < 0) y *= -1;

    int nx = (int)roundf(x) % img->w;
    int ny = (int)roundf(y) % img->w;

    return (SDL_Point){ nx, ny };
}


void prog_reset_heightbuf(struct Prog *p)
{
    for (int i = 0; i < 800; ++i)
        p->heightbuf[i] = 800;
}

