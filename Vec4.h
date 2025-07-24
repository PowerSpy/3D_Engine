#pragma once
#include "Vec3.h"

class Vec4{
public:

    float x, y, z, w;

    Vec4();
    Vec4(float x, float y, float z, float w);
    Vec4(Vec3& v);

    void print() const;
};