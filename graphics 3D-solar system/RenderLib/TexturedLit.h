#pragma once
#include "ShaderProgram.h"
#include "Geometry.h"
#include "Transforms.h"
#include "PointLight.h"
#include "Texture.h"
#include "GeometryHelper.h"
#include "Drawable.h"

class TexturedLit : public Drawable
{
	SOF::ShaderProgram *shader;
	SOF::Geometry *geom;
	SOF::Texture* texture;
	SOF::Texture* normalMap;
	SOF::Cubemap* cubeMap;
	float fEnvMap;
	void CreateGeometry( const GeometryHelper &geometry );
public:
	TexturedLit( const GeometryHelper &geometry, const std::string& textureName="default", const std::string& normalMapName="default" ,
	SOF::Cubemap* ncubeMap=nullptr,float fEnvMap=0.0f) :cubeMap(ncubeMap), fEnvMap(fEnvMap)
	{ 
		CreateGeometry(geometry); 
		// load the textures
		if ( textureName == "default")
			texture = new SOF::Texture("textures/default_diffuse.png");
		else
			texture = new SOF::Texture(textureName);

		if ( normalMapName == "default")
			normalMap = new SOF::Texture("textures/default_normal.png");
		else
			normalMap = new SOF::Texture(normalMapName);
		if (cubeMap == nullptr) {
			//load default and set fmap to 0
			std::vector<std::string> cubeMapFiles;
			cubeMapFiles.push_back("textures/defualt_diffuse.png");
			cubeMapFiles.push_back("textures/defualt_diffuse.png");
			cubeMapFiles.push_back("textures/defualt_diffuse.png");
			cubeMapFiles.push_back("textures/defualt_diffuse.png");
			cubeMapFiles.push_back("textures/defualt_diffuse.png");
			cubeMapFiles.push_back("textures/defualt_diffuse.png");
			cubeMap = new SOF::Cubemap(cubeMapFiles);
			fEnvMap = 0.0f;
		}
	}
	virtual void Draw( const Transforms &renderTransform, const PointLight& light ) override;
};