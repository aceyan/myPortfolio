#pragma once
#include"Matrix4x4.h"
#include"Point2D.h"
#include"Vector3D.h"
#include"Color.h"
#include"Vertex.h"
//Mathematical tools used in the rendering process
static class MathTools
{
public:
	static Matrix4x4 GetTranslate(float x, float y, float z);//Get translation matrix
	static Matrix4x4 GetScale(float x, float y, float z);//Get scale matrix
	static Matrix4x4 GetRotateY(float r);//Get the rotation matrix on the Y axis
	static Matrix4x4 GetRotateX(float r);//Get the rotation matrix on the X axis
	static Matrix4x4 GetRotateZ(float r);//Get the rotation matrix on the Z axis
	static Matrix4x4 GetView(Vector3D pos, Vector3D lookAt, Vector3D up);//get view matrix
	static Matrix4x4 GetProjection(float fov, float aspect, float zn, float zf);//get project matrix
	static float Lerp(float a, float b, float t);//linear interpolation
	static Color Lerp(Color a, Color b, float t);//Interpolate colors linearly
	static void ScreenSpaceLerpVertex(Vertex &v, Vertex v1, Vertex v2, float t);//Interpolate vertex v linearly in screen space
	static int Range(int v, int min, int max);//Controls the range of v values
	static float Range(float v, float min, float max);//Controls the range of v values

};
