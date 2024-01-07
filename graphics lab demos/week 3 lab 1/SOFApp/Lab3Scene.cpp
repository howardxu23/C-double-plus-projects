#include "stdafx.h"
#include "RenderLib.h"
#include <random>

Node* Scene()
{
	QuadGeometry quad(10);
	TexturedLit* floor = new TexturedLit(quad, "textures/marble.png");

	TransformNode* rootNode = new TransformNode(glm::mat4());
	LightNode* light = new LightNode(glm::vec3(20, 50, 50), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.4, 10, 200);
	rootNode->AddChild(light);

	// scale the floor and add 
	TransformNode* floorScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(100, 100, 100)));
	light->AddChild(floorScale);
	floorScale->AddChild(new GeometryNode(floor));

	SphereGeometry sphere(20);
	TexturedLit* sphereRender = new TexturedLit(sphere, "textures/stone.png", "default");
	TransformNode* sphereTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 10.0f, 0.0f)));
	TransformNode* sphereScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(3.0f, 3.0f, 3.0f)));

	light->AddChild(sphereTrans);
	sphereTrans->AddChild(sphereScale);
	sphereScale->AddChild(new GeometryNode(sphereRender));

	return light;
}
