#include "stdafx.h"
#include "RenderLib.h"
#include "spline.h"
#include <random>
#include "physics.h"
Node* Scene(Physics* phys)
{
	QuadGeometry quad(10);
	TexturedLit* floor = new TexturedLit(quad, "textures/marble.png");

	LightNode* light = new LightNode(glm::vec3(0, 100, 0), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.4, 10, 200);

	GeometryNode* geomNode = new GeometryNode(floor);
	// scale the floor and add 
	glm::mat4 floorMat = glm::scale(glm::mat4(), glm::vec3(100, 100, 100));
	TransformNode* floorScale = new TransformNode(floorMat);
	light->AddChild(floorScale);
	floorScale->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(floorMat));
	// make some walls
	glm::mat4 wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(50.0, 0.0, 0.0));
	wtrans = glm::rotate(wtrans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 100.0f, 100.0f));
	TransformNode *wall0 = new TransformNode(wtrans);
	
	light->AddChild(wall0);
	wall0->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(-50.0, 0.0, 0.0));
	wtrans = glm::rotate(wtrans, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 100.0f, 100.0f));
	TransformNode* wall1 = new TransformNode(wtrans);
	light->AddChild(wall1);
	wall1->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(0.0, 0.0, 50.0));
	wtrans = glm::rotate(wtrans, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 100.0f, 100.0f));
	TransformNode* wall2 = new TransformNode(wtrans);
	light->AddChild(wall2);
	wall2->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(0.0, 0.0, -50.0));
	wtrans = glm::rotate(wtrans, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0));
	wtrans = glm::scale(wtrans, glm::vec3(100.0f, 100.0f, 100.0f));
	TransformNode* wall3 = new TransformNode(wtrans);
	light->AddChild(wall3);
	wall3->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));

	return light;
}
