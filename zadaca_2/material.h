#pragma once
#include "geometry.h"

struct Material
{
    Vec3f diffuse_color;

    float diffuse_coef = 1;
    float specular_coef = 1;
    float phong_exp = 1;
    float reflex_coef = 0.2;
    float refract_coef = 1;
    float opacity = 1;

    Material(const Vec3f &color) : diffuse_color(color) {}
    Material() : diffuse_color(Vec3f(0, 0, 0)) {}
};