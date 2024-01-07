#pragma once
#include "stdafx.h"
#include "app.h"
#include "texturedlit.h"
#include "Emissive.h"
#include "FlyCam.h"
#include "MeshObject.h"

class Week2App : public SOF::App
{
	// this app will render spheres in random positions.
	// the lab exercise is to texture them, and then add a point light
	// which we can move around the scene using the keyboard input.
	const static int numObjects = 100;
	glm::vec4 objectPositions[numObjects]; // radius is stored in the w component
	TexturedLit *object; // archetype for the sphere instances
	Emissive* lightSphere; // we will use this to show the light position
	glm::vec3 lightPos[4];
	glm::vec4 lightColors[4];
	float lightRadius[4];
	FlyCam* camera;
	MeshObject* mesh;
	TexturedLit* floorQuad;
	TexturedLit* cylinder;
public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};