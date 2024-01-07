#pragma once
#include "stdafx.h"
#include "app.h"

class ColourCubeApp : public SOF::App
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;
	float angle=0.0f;
public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};