#pragma once

#include <cmath>
#include <imgui.h>

struct Vec2
{
	float X;
	float Y;

	Vec2() = default;
	Vec2(float X, float Y) : X(X), Y(Y) { }

    float DistanceTo(const Vec2 &Other) const
	{
        float DX = X - Other.X;
        float DY = Y - Other.Y;
        return sqrtf(DX * DX + DY * DY);
    }
};

struct Vec3
{
	float X;
	float Y;
	float Z;

	Vec3(float X = 0, float Y = 0, float Z = 0) : X(X), Y(Y), Z(Z) { }

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

    float Distance(const Vec3 &Other) const
	{
		float DX = X - Other.X;
		float DY = Y - Other.Y;
		float DZ = Z - Other.Z;
		return std::sqrt(DX * DX + DY * DY + DZ * DZ);
	}
};

inline ImVec2 ConvertVec2ToImVec2(const Vec2 &Vec)
{
    return ImVec2(Vec.X, Vec.Y);
}
