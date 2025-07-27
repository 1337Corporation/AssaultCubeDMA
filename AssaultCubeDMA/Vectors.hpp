#pragma once

#include <cmath>

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
