#pragma once
#include"Vector3D.h"
#include"Color.h"
struct Light
{
	
	Vector3D worldPosition;//The world position of the light
	Color lightColor;//color of the light
	Light(){};
	Light(Vector3D wp, Color lc)
	{
		 worldPosition = wp;
		 lightColor = lc;
	}
};