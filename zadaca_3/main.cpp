#include <cmath>
#include "tgaimage.h"

using namespace std;

// dimenzije slike
const int width  = 512;
const int height = 512;

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

typedef float Vec3[3];
typedef float Vec2[2];


void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(x, y, color);
}


float coord_calc(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

float area(float x1, float y1, float x2, float y2, float x3, float y3)
{
    return abs(( x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0 );
}

bool in_triangle(float x1, float y1, float x2, float y2, float x3, float y3, float x, float y)
{
    float A  = area (x1, y1, x2, y2, x3, y3);
    float A1 = area (x, y, x2, y2, x3, y3);
    float A2 = area (x1, y1, x, y, x3, y3);
    float A3 = area (x1, y1, x2, y2, x, y);
    return (A == A1 + A2 + A3);
}


void draw_triangle_2d(float x0, float y0, float x1, float y1, float x2, float y2, TGAImage& image, TGAColor color)
{
    int maxX = ceil  (max (max(x0, x1), x2));
    int minX = floor (min (min(x0, x1), x2));
    int maxY = ceil  (max (max(y0, y1), y2));
    int minY = floor (min (min(y0, y1), y2));

    for (int j = minY; j <= maxY ; ++j)
        for (int i = minX; i <= maxX; ++i)
            if (in_triangle(x0, y0, x1, y1, x2, y2, i, j))
                set_color(j, i, image, color);
}


void draw_triangle_2d_gouraurd(float x0, float y0, const TGAColor& c0, float x1, float y1, const TGAColor& c1, float x2, float y2, TGAImage& image, const TGAColor& c2)
{
    TGAColor color;

    int maxX = ceil  (max (max(x0, x1), x2));
    int minX = floor (min (min(x0, x1), x2));
    int maxY = ceil  (max (max(y0, y1), y2));
    int minY = floor (min (min(y0, y1), y2));

    for (int j = minY; j <= maxY; ++j)
    {
        for (int i = minX; i <= maxX; ++i)
        {
            float fst = coord_calc(x1, y1, x2, y2, i, j) / coord_calc(x1, y1, x2, y2, x0, y0);
            float snd = coord_calc(x2, y2, x0, y0, i, j) / coord_calc(x2, y2, x0, y0, x1, y1);
            float trd = coord_calc(x0, y0, x1, y1, i, j) / coord_calc(x0, y0, x1, y1, x2, y2);

            if (in_triangle(x0, y0, x1, y1, x2, y2, i, j))
            {
                color.r = fst*c0.r + snd*c1.r + trd*c2.r;
                color.g = fst*c0.g + snd*c1.g + trd*c2.g;
                color.b = fst*c0.b + snd*c1.b + trd*c2.b;
                set_color(i, j, image, color);
            }
        }
    }
}


void draw_triangle(const Vec3 v1, const Vec3 v2, const Vec3 v3, TGAImage& image, const TGAColor& color)
{

    float x1 = (1 + (v1[0] / v1[2])) * 0.5 * width;
    float x2 = (1 + (v1[1] / v1[2])) * 0.5 * height;
    float y1 = (1 + (v2[0] / v2[2])) * 0.5 * width;
    float y2 = (1 + (v2[1] / v2[2])) * 0.5 * height;
    float z1 = (1 + (v3[0] / v3[2])) * 0.5 * width;
    float z2 = (1 + (v3[1] / v3[2])) * 0.5 * height;

    Vec2 v1_new = { x1, x2};
    Vec2 v2_new = { y1, y2 };
    Vec2 v3_new = { z1, z2 };

    int maxX = ceil  (max (max(v1_new[0], v2_new[0]), v3_new[0]));
    int minX = floor (min (min(v1_new[0], v2_new[0]), v3_new[0]));
    int maxY = ceil  (max (max(v1_new[1], v2_new[1]), v3_new[1]));
    int minY = floor (min (min(v1_new[1], v2_new[1]), v3_new[1]));

    for (int j = minY; j <= maxY; ++j)
    {
        for (int i = minX; i <= maxX; ++i)
        {
            float fst = coord_calc(v2_new[0], v2_new[1], v3_new[0], v3_new[1], i, j) / coord_calc(v2_new[0], v2_new[1], v3_new[0], v3_new[1], v1_new[0], v1_new[1]);
            float snd = coord_calc(v3_new[0], v3_new[1], v1_new[0], v1_new[1], i, j) / coord_calc(v3_new[0], v3_new[1], v1_new[0], v1_new[1], v2_new[0], v2_new[1]);
            float trd = coord_calc(v1_new[0], v1_new[1], v2_new[0], v2_new[1], i, j) / coord_calc(v1_new[0], v1_new[1], v2_new[0], v2_new[1], v3_new[0], v3_new[1]);

            if (fst > 0 && fst < 1 && snd > 0 && snd < 1 && trd > 0 && trd < 1)
                set_color(i, j, image, color);
        }
    }
}


void draw_triangle_tex(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, TGAImage& image, const char* path)
{
    TGAImage texture;
    texture.read_tga_file(path);

    Vec3 A {x0, y0,z0};
    Vec3 B {x1, y1,z1};
    Vec3 C {x2, y2,z2};

    Vec2 uv0 {u0, v0};
    Vec2 uv1 {u1, v1};
    Vec2 uv2 {u2, v2};

    float a = (1 + (A[0] / A[2])) * 0.5 * width;
    float b = (1 + (A[1] / A[2])) * 0.5 * height;
    float c = (1 + (B[0] / B[2])) * 0.5 * width;
    float d = (1 + (B[1] / B[2])) * 0.5 * height;
    float e = (1 + (C[0] / C[2])) * 0.5 * width;
    float f = (1 + (C[1] / C[2])) * 0.5 * height;


    Vec3 A_new = { a, b, 1 / A[2] };
    Vec3 B_new = { c, d, 1 / B[2] };
    Vec3 C_new = { e, f, 1 / C[2] };


    uv0[0] /= A[2]; uv0[1] /= A[2];
    uv1[0] /= B[2]; uv1[1] /= B[2];
    uv2[0] /= C[2]; uv2[1] /= C[2];

    for (int j = 0; j <= width; ++j)
    {
        for (int i = 0; i <= height; ++i)
        {
            float fst = coord_calc(B_new[0], B_new[1], C_new[0], C_new[1], i, j) / coord_calc(B_new[0], B_new[1], C_new[0], C_new[1], A_new[0], A_new[1]);
            float snd = coord_calc(C_new[0], C_new[1], A_new[0], A_new[1], i, j) / coord_calc(C_new[0], C_new[1], A_new[0], A_new[1], B_new[0], B_new[1]);
            float trd = coord_calc(A_new[0], A_new[1], B_new[0], B_new[1], i, j) / coord_calc(A_new[0], A_new[1], B_new[0], B_new[1], C_new[0], C_new[1]);

            if((fst >= 0 && snd >= 0 && trd >= 0))
            {
                float x = texture.get_width() * (fst * u0 + snd * u1 + trd * u2);
                float y = texture.get_height() * (fst * v0 + snd * v1 + trd * v2);
                set_color(i, j, image, texture.get(round(x), round(y)));
            }
        }
    }
}


void draw_triangle_tex_corrected(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2, float u0, float v0, float u1, float v1, float u2, float v2, TGAImage& image, const char* path)
{
    TGAImage texture;
    texture.read_tga_file(path);

    Vec3 A {x0, y0,z0};
    Vec3 B {x1, y1,z1};
    Vec3 C {x2, y2,z2};

    Vec2 uv0 {u0, v0};
    Vec2 uv1 {u1, v1};
    Vec2 uv2 {u2, v2};

    float a = (1 + (A[0] / A[2])) * 0.5 * width;
    float b = (1 + (A[1] / A[2])) * 0.5 * height;
    float c = (1 + (B[0] / B[2])) * 0.5 * width;
    float d = (1 + (B[1] / B[2])) * 0.5 * height;
    float e = (1 + (C[0] / C[2])) * 0.5 * width;
    float f = (1 + (C[1] / C[2])) * 0.5 * height;


    Vec3 A_new = { a, b, 1 / A[2] };
    Vec3 B_new = { c, d, 1 / B[2] };
    Vec3 C_new = { e, f, 1 / C[2] };


    uv0[0] /= A[2]; uv0[1] /= A[2];
    uv1[0] /= B[2]; uv1[1] /= B[2];
    uv2[0] /= C[2]; uv2[1] /= C[2];

    for (int j = 0; j <= width; ++j)
    {
        for (int i = 0; i <= height; ++i)
        {
            float fst = coord_calc(B_new[0], B_new[1], C_new[0], C_new[1], i, j) / coord_calc(B_new[0], B_new[1], C_new[0], C_new[1], A_new[0], A_new[1]);
            float snd = coord_calc(C_new[0], C_new[1], A_new[0], A_new[1], i, j) / coord_calc(C_new[0], C_new[1], A_new[0], A_new[1], B_new[0], B_new[1]);
            float trd = coord_calc(A_new[0], A_new[1], B_new[0], B_new[1], i, j) / coord_calc(A_new[0], A_new[1], B_new[0], B_new[1], C_new[0], C_new[1]);

            if (fst > 0 && fst < 1 && snd > 0 && snd < 1 && trd > 0 && trd < 1)
            {
                float x = (fst * uv0[0] + snd * uv1[0] + trd * uv2[0]) * texture.get_width();
                float y = (fst * uv0[1] + snd * uv1[1] + trd * uv2[1]) * texture.get_height();
                float z  = 1 / (fst * A_new[2] + snd * B_new[2] + trd * C_new[2]);
                x *= z; y *= z;
                set_color(i, j, image, texture.get(round(x), round(y)));
            }
        }
    }
}

int main()
{
    TGAImage image(width, height, TGAImage::RGB);

    draw_triangle_2d(120, 230, 200, 230, 100, 120, image, green);
    draw_triangle_2d(300, 25, 500, 45, 450, 100, image, white);
    draw_triangle_2d_gouraurd(220, 230, white, 420, 230, red, 220, 430, image, blue);

    image.flip_vertically();
    image.write_tga_file("zad1zad2.tga");

    TGAImage image2(width, height, TGAImage::RGB);
    Vec3 vertex1{-48,-10, 82};
    Vec3 vertex2{29, -15, 44};
    Vec3 vertex3{13, 34, 114};
    draw_triangle(vertex1, vertex2, vertex3, image2, blue);

    image2.flip_vertically();
    image2.write_tga_file("zad3.tga");

    TGAImage image3(width, height, TGAImage::RGB);
    draw_triangle_tex(-48, -10,  92, 29, -15,  44, 13,  34, 114, 0, 0, 0, 1, 1, 0, image3, "tgaimage.tga");

    image3.flip_vertically();
    image3.write_tga_file("zad4a.tga");

    TGAImage image4(width, height, TGAImage::RGB);
    draw_triangle_tex_corrected(-48, -10,  82, 29, -15,  44, 13,  34, 114, 0, 0, 0, 1, 1, 0, image4, "tgaimage.tga");

    image4.flip_vertically();
    image4.write_tga_file("zad4b.tga");
}