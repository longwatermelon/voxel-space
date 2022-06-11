#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

struct Camera
{
    Vec2f pos;
    float height;
    float angle, pitch;
};

struct Camera *cam_alloc(Vec2f pos, float height, float angle, float pitch);
void cam_free(struct Camera *c);

#endif

