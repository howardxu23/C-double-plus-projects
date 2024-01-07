#pragma once
#include "stdafx.h"
#include "app.h"
#include "RenderLib.h"
#include "RobotArm.h"

#define TEST_CUBOID 0

class Week3App1 : public SOF::App
{
	TexturedLit* floorQuad;
	PointLight lights;
	void SetupLights();

	// camera
	glm::vec3 eye;    
	glm::vec3 lookAt; 
	glm::vec3 up;     
	FlyCam* camera;
#if TEST_CUBOID
	TexturedLit* cuboid;
#else
	RobotArm* arm;
#endif
public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};