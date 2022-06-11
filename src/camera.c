#include "camera.h"
#include <stdlib.h>


struct Camera *cam_alloc(Vec2f pos, float height, float angle)
{
    struct Camera *c = malloc(sizeof(struct Camera));
    c->pos = pos;
    c->angle = angle;
    c->height = height;

    return c;
}


void cam_free(struct Camera *c)
{
    free(c);
}

