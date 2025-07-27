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

	Vec3(float x, float y, float z) : X(x), Y(y), Z(z) { }

	Vec3 operator+(const Vec3 &Other) const
	{
		return Vec3(X + Other.X, Y + Other.Y, Z + Other.Z);
	}

	Vec3 operator-(const Vec3 &Other) const
	{
		return Vec3(X - Other.X, Y - Other.Y, Z - Other.Z);
	}

	Vec3 operator/(float Scalar) const
	{
		return Vec3(X / Scalar, Y / Scalar, Z / Scalar);
	}

	Vec3 operator*(float Scalar) const
	{
		return Vec3(X * Scalar, Y * Scalar, Z * Scalar);
	}

    float Distance(const Vec3& Other) const
	{
		float Dx = X - Other.X;
		float Dy = Y - Other.Y;
		float Dz = Z - Other.Z;
		return std::sqrt(Dx * Dx + Dy * Dy + Dz * Dz);
	}
};
