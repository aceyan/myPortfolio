#pragma once
#include"Vector3D.h"
#include"Color.h"
struct Vertex
{
	Vector3D point;//Vertex coordinates
	float u;//Texture coordinates
	float v;//Texture coordinates
	Color vcolor; //Vertex color
	Vector3D normal;//vertex normal
	Color lightingColor;//Used to save the color of the vertex after lighting
	//
	float onePerZ;//1/z£¬used for perspective correction

	Vertex()
	{
		point = Vector3D();
		u = v = 0;
		onePerZ = 1;
		vcolor = Color();
		normal = Vector3D(1,1,1);
		lightingColor = Color(0, 0, 0);
	}

	Vertex(Vector3D p, Vector3D n, float u, float v, Color color) :point(p), normal(n), u(u), v(v), vcolor(color)
	{

		point.w = 1;
		onePerZ = 1;

		lightingColor.SetR(0);
		lightingColor.SetB(0);
		lightingColor.SetG(0);
	};
};