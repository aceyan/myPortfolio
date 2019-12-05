#pragma once
#include"Vector3D.h"
struct Camera
{
public:
	Vector3D pos;//Camera position
	Vector3D lookAt;//Camera direction
	Vector3D up;//the up direction
	float fov; // Observation Angle (in radian)
	float aspect;//Aspect ratio
	float zn;//near  plane
	float zf;//far plane
	Camera(){};
	Camera(Vector3D pos, Vector3D lookAt, Vector3D up, float fov, float aspect, float zn, float zf) :pos(pos), lookAt(lookAt), up(up), fov(fov), aspect(aspect), zn(zn), zf(zf)
	{
	};
};