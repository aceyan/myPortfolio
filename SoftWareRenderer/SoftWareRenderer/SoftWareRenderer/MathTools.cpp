#include"MathTools.h"
Matrix4x4  MathTools::GetTranslate(float x, float y, float z)
{
	return Matrix4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1);
}

Matrix4x4  MathTools::GetScale(float x, float y, float z)
{
	return  Matrix4x4(x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1);
}

Matrix4x4  MathTools::GetRotateY(float r)
{
	Matrix4x4 rm;
	rm.Identity();
	rm.m[0][0] = cos(r);

	rm.m[0][2] = -sin(r);
	//

	rm.m[2][0] = sin(r);
	rm.m[2][2] = cos(r);
	return rm;
}

Matrix4x4  MathTools::GetRotateX(float r)
{
	Matrix4x4 rm;
	rm.Identity();
	rm.m[1][1] = cos(r);
	rm.m[1][2] = sin(r);
	//

	rm.m[2][1] = -sin(r);
	rm.m[2][2] = cos(r);
	return rm;
}

Matrix4x4  MathTools::GetRotateZ(float r)
{
	Matrix4x4 rm;
	rm.Identity();
	rm.m[0][0] = cos(r);
	rm.m[0][1] = sin(r);
	//
	rm.m[1][0] = -sin(r);
	rm.m[1][1] = cos(r);
	return rm;
}


Matrix4x4 MathTools::GetView(Vector3D pos, Vector3D lookAt, Vector3D up)
{
	//direction of camera
	Vector3D dir = lookAt - pos;
	Vector3D right = Vector3D::Cross(up, dir);
	right.Normalize();
	//
	Matrix4x4 t =  Matrix4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-pos.x, -pos.y, -pos.z, 1);
	//
	Matrix4x4 r =  Matrix4x4(right.x, up.x, dir.x, 0,
		right.y, up.y, dir.y, 0,
		right.z, up.z, dir.z, 0,
		0, 0, 0, 1);
	return t * r;
}

Matrix4x4 MathTools::GetProjection(float fov, float aspect, float zn, float zf)
{
	Matrix4x4 p =  Matrix4x4();
	p.SetZero();
	p.m[0][0] = (float)(1 / (tan(fov * 0.5f) * aspect));
	p.m[1][1] = (float)(1 / tan(fov * 0.5f));
	p.m[2][2] = zf / (zf - zn);
	p.m[2][3] = (float)1;
	p.m[3][2] = (zn * zf) / (zn - zf);
	return p;
}

float MathTools::Lerp(float a, float b, float t)
{
	if (t <= 0)
	{
		return a;
	}
	else if (t >= 1)
	{
		return b;
	}
	else
	{
		return b * t + (1 - t) * a;
	}
}

Color MathTools::Lerp(Color a, Color b, float t)
{
	if (t <= 0)
	{
		return a;
	}
	else if (t >= 1)
	{
		return b;
	}
	else
	{
		return t * b + (1 - t) * a;
	}
}

void MathTools::ScreenSpaceLerpVertex(Vertex &v, Vertex v1, Vertex v2, float t)
{
	v.onePerZ = MathTools::Lerp(v1.onePerZ, v2.onePerZ, t);
	//
	v.u = MathTools::Lerp(v1.u, v2.u, t);
	v.v = MathTools::Lerp(v1.v, v2.v, t);
	//
	v.vcolor = MathTools::Lerp(v1.vcolor, v2.vcolor, t);
	//
	v.lightingColor = MathTools::Lerp(v1.lightingColor, v2.lightingColor, t);
}

int MathTools::Range(int v, int min, int max)
{
	if (v <= min)
	{
		return min;
	}
	if (v >= max)
	{
		return max;
	}
	return v;
}
float MathTools::Range(float v, float min, float max)
	{
		if (v <= min)
		{
			return min;
		}
		if (v >= max)
		{
			return max;
		}
		return v;
	}