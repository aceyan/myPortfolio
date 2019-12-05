#pragma once
#include"Vertex.h"
#include"Material.h"
#include"Point2D.h"
#include"Vector3D.h"
#include<vector>
using namespace std;
//Defines the data for a cube
struct CubeMeshData
{
public:

	 CubeMeshData()
	 {
		 //Vertex coordinates
		 Vector3D pointList[8] = {
			 Vector3D(-1, 1, -1),
			 Vector3D(-1, -1, -1),
			 Vector3D(1, -1, -1),
			 Vector3D(1, 1, -1),

			 Vector3D(-1, 1, 1),
			 Vector3D(-1, -1, 1),
			 Vector3D(1, -1, 1),
			 Vector3D(1, 1, 1)
		 };
		 //Triangle vertices index - 12 planes
		 int indexs[36] = { 0, 1, 2,
			 0, 2, 3,
			 //
			 7, 6, 5,
			 7, 5, 4,
			 //
			 0, 4, 5,
			 0, 5, 1,
			 //
			 1, 5, 6,
			 1, 6, 2,
			 //
			 2, 6, 7,
			 2, 7, 3,
			 //
			 3, 7, 4,
			 3, 4, 0
		 };

		 //uv coordinates
		 Point2D uvs[36] = {
			 Point2D(0, 0), Point2D(0, 1), Point2D(1, 1),
			 Point2D(0, 0), Point2D(1, 1), Point2D(1, 0),
			 //
			 Point2D(0, 0), Point2D(0, 1), Point2D(1, 1),
			 Point2D(0, 0), Point2D(1, 1), Point2D(1, 0),
			 //
			 Point2D(0, 0), Point2D(0, 1), Point2D(1, 1),
			 Point2D(0, 0), Point2D(1, 1), Point2D(1, 0),
			 //
			 Point2D(0, 0), Point2D(0, 1), Point2D(1, 1),
			 Point2D(0, 0), Point2D(1, 1), Point2D(1, 0),
			 //
			 Point2D(0, 0), Point2D(0, 1), Point2D(1, 1),
			 Point2D(0, 0), Point2D(1, 1), Point2D(1, 0),
			 ///
			 Point2D(0, 0), Point2D(0, 1), Point2D(1, 1),
			 Point2D(0, 0), Point2D(1, 1), Point2D(1, 0)
		 };
		 //Vertex color
		 Color vertColors[36] = {
			 Color(0, 255, 0), Color(0, 0, 255), Color(255, 0, 0),
			 Color(0, 255, 0), Color(255, 0, 0), Color(0, 0, 255),
			 //
			 Color(0, 255, 0), Color(0, 0, 255), Color(255, 0, 0),
			 Color(0, 255, 0), Color(255, 0, 0), Color(0, 0, 255),
			 //
			 Color(0, 255, 0), Color(0, 0, 255), Color(255, 0, 0),
			 Color(0, 255, 0), Color(255, 0, 0), Color(0, 0, 255),
			 //
			 Color(0, 255, 0), Color(0, 0, 255), Color(255, 0, 0),
			 Color(0, 255, 0), Color(255, 0, 0), Color(0, 0, 255),
			 //
			 Color(0, 255, 0), Color(0, 0, 255), Color(255, 0, 0),
			 Color(0, 255, 0), Color(255, 0, 0), Color(0, 0, 255),
			 //
			 Color(0, 255, 0), Color(0, 0, 255), Color(255, 0, 0),
			 Color(0, 255, 0), Color(255, 0, 0), Color(0, 0, 255)
		 };
		 //normals of vertex
		 Vector3D normals[36] = {
			 Vector3D(0, 0, -1), Vector3D(0, 0, -1), Vector3D(0, 0, -1),
			 Vector3D(0, 0, -1), Vector3D(0, 0, -1), Vector3D(0, 0, -1),
			 //
			 Vector3D(0, 0, 1), Vector3D(0, 0, 1), Vector3D(0, 0, 1),
			 Vector3D(0, 0, 1), Vector3D(0, 0, 1), Vector3D(0, 0, 1),
			 //
			 Vector3D(-1, 0, 0), Vector3D(-1, 0, 0), Vector3D(-1, 0, 0),
			 Vector3D(-1, 0, 0), Vector3D(-1, 0, 0), Vector3D(-1, 0, 0),
			 //
			 Vector3D(0, -1, 0), Vector3D(0, -1, 0), Vector3D(0, -1, 0),
			 Vector3D(0, -1, 0), Vector3D(0, -1, 0), Vector3D(0, -1, 0),
			 //
			 Vector3D(1, 0, 0), Vector3D(1, 0, 0), Vector3D(1, 0, 0),
			 Vector3D(1, 0, 0), Vector3D(1, 0, 0), Vector3D(1, 0, 0),
			 //
			 Vector3D(0, 1, 0), Vector3D(0, 1, 0), Vector3D(0, 1, 0),
			 Vector3D(0, 1, 0), Vector3D(0, 1, 0), Vector3D(0, 1, 0)
		 };




		 //Generating a list of vertices
		 for (int i = 0; i < 36; i++)
		 {
			 int pointIndex = indexs[i];
			 Vector3D point = pointList[pointIndex];
			 verts.push_back(Vertex(point, normals[i], uvs[i].x, uvs[i].y, vertColors[i]));
		 }
	 };

	 vector<Vertex> verts;
	 Material material = Material(Color(10, 10, 10), 0.1f, Color(120, 120, 120), Color(255, 255, 255), 99);

	
};