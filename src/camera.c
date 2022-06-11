#include "camera.h"
#include <stdlib.h>


struct Camera *cam_alloc(Vec2f pos, float height, float angle, float pitch)
{
    struct Camera *c = malloc(sizeof(struct Camera));
    c->pos = pos;
    c->angle = angle;
    c->height = height;
    c->pitch = pitch;

    return c;
}


void cam_free(struct Camera *c)
{
    free(c);
}

