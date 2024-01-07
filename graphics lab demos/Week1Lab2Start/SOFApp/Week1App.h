#pragma once
#include "stdafx.h"
#include "app.h"
#include "TexturedLitObject.h"
#include "EmissiveObject.h"

class Week1App : public SOF::App
{
	// this app will render spheres in random positions.
	// the lab exercise is to texture them, and then add a point light
	// which we can move around the scene using the keyboard input.
	const static int numSpheres = 100;
	glm::vec4 spherePositions[numSpheres]; // radius is stored in the w component
	TexturedLitObject*sphere; // archetype for the sphere instances
	EmmisiveObject* lightSphere; // we will use this to show the light position
	glm::vec3 lightPos;

public:
	virtual void Init();
	virtual void Draw();
	virtual void Update(float deltaT);
};