#include "stdafx.h"
#include "RenderLib.h"
#include "spline.h"
#include <random>

std::vector<glm::vec3> splinePoints;

Node* Scene()
{
	QuadGeometry quad(10);
	TexturedLit* floor = new TexturedLit(quad, "textures/marble.png");

	TransformNode* rootNode = new TransformNode(glm::mat4());
	LightNode* light = new LightNode(glm::vec3(20, 50, -50), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.4, 10, 200);
	rootNode->AddChild(light);

	// scale the floor and add 
	TransformNode* floorScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(100, 100, 100)));
	light->AddChild(floorScale);
	floorScale->AddChild(new GeometryNode(floor));

	std::vector<glm::vec3> lineVerts;

	splinePoints.push_back(glm::vec3(-6, 1, -6));
	splinePoints.push_back(glm::vec3(6, 1, 18) );
	splinePoints.push_back(glm::vec3(24, 1, 6) );
	splinePoints.push_back(glm::vec3(24, 1, -12) );
	splinePoints.push_back(glm::vec3(24, 1, -21) );
	splinePoints.push_back(glm::vec3(0, 1, -30) );
	splinePoints.push_back(glm::vec3(-18, 1, -18) );
	splinePoints.push_back(glm::vec3(-18, 1, 18) );

	SphereGeometry sphere(20);
	TexturedLit* sphereRender = new TexturedLit(sphere, "textures/grid.jpg");
	GeometryNode* sphereNode = new GeometryNode(sphereRender);
	for (glm::vec3 pos : splinePoints)
	{
		TransformNode* sphereTrans = new TransformNode(glm::translate(glm::mat4(), pos));
		light->AddChild(sphereTrans);
		sphereTrans->AddChild(sphereNode);
	}
#if 0
	PolyLine* p = new PolyLine(splinePoints, glm::vec4(1, 0, 0, 1));
	light->AddChild(new GeometryNode(p));
#endif
	CRspline c(splinePoints, true,0.5f);
	for (int i = 0; i < 500; i++)
	{
		float u = 8.0f * i / 499.0f;
		lineVerts.push_back(c.GetPoint(u));
	}
	PolyLine* p = new PolyLine(lineVerts, glm::vec4(1, 0, 0, 1));
	light->AddChild(new GeometryNode(p));
	return light;
}
