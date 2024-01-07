#pragma once
#include "stdafx.h"
#include "app.h"
#include "RenderLib.h"
#include "spline.h"

class Week4App : public SOF::App
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
	// camera spline
	CRspline* camSpline;
	float splineParameter = 0.0f;
	float cameraSpeed = 0.5f;
public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};