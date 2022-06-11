#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"

struct Camera
{
    Vec2f pos;
    float angle;
};

struct Camera *cam_alloc(Vec2f pos, float angle);
void cam_free(struct Camera *c);

#endif

