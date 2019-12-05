#pragma once
#include<Windows.h>
#include"Matrix4x4.h"
#include<math.h>

struct Vector3D
{
public:
	float x;
	float y;
	float z;
	float w;

	Vector3D():x(0), y(0), z(0), w(0){};
	Vector3D(float x, float y, float z, float w) : x(x), y(y), z(z), w(w){};
	Vector3D(float x, float y, float z) : x(x), y(y), z(z), w(0){};
	float GetLength()
	{
		float sq = x * x + y * y + z * z;
		return (float)sqrt(sq);
	};
	void Normalize()
	{
		float length = GetLength();
		if (length != 0)
		{
			float s = 1 / length;
			x *= s;
			y *= s;
			z *= s;
		}
	};
	friend Vector3D operator *(Vector3D lhs, Matrix4x4 rhs)
	{
		Vector3D v;
		v.x = lhs.x * rhs.m[0][0] + lhs.y * rhs.m[1][0] + lhs.z * rhs.m[2][0] + lhs.w * rhs.m[3][0];
		v.y = lhs.x * rhs.m[0][1] + lhs.y * rhs.m[1][1] + lhs.z * rhs.m[2][1] + lhs.w * rhs.m[3][1];
		v.z = lhs.x * rhs.m[0][2] + lhs.y * rhs.m[1][2] + lhs.z * rhs.m[2][2] + lhs.w * rhs.m[3][2];
		v.w = lhs.x * rhs.m[0][3] + lhs.y * rhs.m[1][3] + lhs.z * rhs.m[2][3] + lhs.w * rhs.m[3][3];
		return v;
	};
	friend Vector3D operator -(Vector3D lhs, Vector3D rhs)
	{
		return Vector3D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, 0);
	};
	friend Vector3D operator +(Vector3D lhs, Vector3D rhs)
	{
		return Vector3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, 0);
	};
	static float Dot(Vector3D lhs, Vector3D rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	};
	static Vector3D Cross(Vector3D lhs, Vector3D rhs)
	{
		float x = lhs.y * rhs.z - lhs.z * rhs.y;
		float y = lhs.z * rhs.x - lhs.x * rhs.z;
		float z = lhs.x * rhs.y - lhs.y * rhs.x;
		return Vector3D(x, y, z, 0);
	};


};
