#pragma once
#include "stdafx.h"
#include "app.h"
#include "RenderLib.h"
#include "physics.h"

class Week6App : public SOF::App
{
	// geometry
	TexturedLit* floorQuad;

	// camera
	glm::vec3 eye;    
	glm::vec3 lookAt; 
	glm::vec3 up;     
	FlyCam* camera;

	// scene graph
	Node* sceneGraphRoot;
	//physics
	Physics* physics;
public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};