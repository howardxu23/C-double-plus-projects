#pragma once
#include "stdafx.h"
#include "app.h"
#include "RenderLib.h"

class Week3App2 : public SOF::App
{
	// geometry
	TexturedLit* floorQuad;
	TexturedLit* cuboid;

	// camera
	glm::vec3 eye;    
	glm::vec3 lookAt; 
	glm::vec3 up;     
	FlyCam* camera;

	// scene graph
	Node* sceneGraphRoot;

public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};