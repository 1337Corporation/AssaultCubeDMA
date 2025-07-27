#pragma once

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define RAD2DEG (180.0f / 3.14159265f)
#define DEG2RAD (3.14159265358979323846f / 180.0f)

struct Vec2
{
	float X;
	float Y;

	Vec2() = default;
	Vec2(float x, float y) : X(x), Y(y) { }

    float DistanceTo(const Vec2& other) const
	{
        float Dx = X - other.X;
        float Dy = Y - other.Y;
        return sqrtf(Dx * Dx + Dy * Dy);
    }
};

struct Vec3
{
	float X;
	float Y;
	float Z;
};
