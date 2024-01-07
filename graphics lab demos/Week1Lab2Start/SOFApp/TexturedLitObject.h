#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Transforms.h"
#include "PointLight.h"
#include "Texture.h"
#include "geometryHelper.h"
//
// Sphere - sets up the geometry and shader program for a tesselated sphere.
// Week 2 lab (6G6Z2109)
//

// this is a unit sphere, centred on the origin (use scale to change the size)

class TexturedLitObject
{
	SOF::ShaderProgram *shader;
	SOF::Geometry *geom;
	SOF::Texture* texture;
	void CreateGeometry(const GeometryHelper& geometry);
public:
	TexturedLitObject(const GeometryHelper &geometry,const std::string& textureName) { 
		CreateGeometry(geometry);
		//load texture
		texture = new SOF::Texture(textureName);
	}
	void Draw( const Transforms &renderTransform,  const PointLight& light);
};