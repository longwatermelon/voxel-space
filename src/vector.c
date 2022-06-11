#include "vector.h"
#include <math.h>


float vec_len(Vec2f a)
{
    return sqrtf(a.x * a.x + a.y * a.y);
}


Vec2f vec_normalize(Vec2f a)
{
    float len = vec_len(a);

    return (Vec2f){
        a.x / len,
        a.y / len
    };
}


Vec2f vec_addv(Vec2f a, Vec2f b)
{
    return (Vec2f){
        a.x + b.x,
        a.y + b.y
    };
}


Vec2f vec_subv(Vec2f a, Vec2f b)
{
    return (Vec2f){
        a.x - b.x,
        a.y - b.y
    };
}


Vec2f vec_mulf(Vec2f a, float t)
{
    return (Vec2f){
        a.x * t,
        a.y * t
    };
}


float vec_cross(Vec2f a, Vec2f b)
{
    return a.x * b.y - b.x * a.y;
}


float vec_dot(Vec2f a, Vec2f b)
{
    return a.x * b.x + a.y * b.y;
}

