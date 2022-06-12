#include "camera.h"
#include <stdlib.h>


struct Camera *cam_alloc(Vec2f pos, float height, float angle, float pitch, float tilt)
{
    struct Camera *c = malloc(sizeof(struct Camera));
    c->pos = pos;
    c->angle = angle;
    c->height = height;
    c->pitch = pitch;
    c->tilt = tilt;

    return c;
}


void cam_free(struct Camera *c)
{
    free(c);
}

