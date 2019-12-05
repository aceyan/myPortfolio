#pragma once
#include"Color.h"
struct Material
{
	Color emissive;//emissive color
    float ka;//Ambient light reflection factor
	 Color diffuse;//diffuse color
	 Color specular;//specular color
	 float shininess;//specular shiniess
	 Material(Color emissive, float ka, Color diffuse, Color specular, float shininess) :emissive(emissive), ka(ka), diffuse(diffuse), specular(specular), shininess(shininess)
	{
	 };
};