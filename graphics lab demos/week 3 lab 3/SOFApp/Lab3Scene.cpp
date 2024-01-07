#include "stdafx.h"
#include "RenderLib.h"
#include <random>

Node* Scene()
{
	QuadGeometry quad(10);
	TexturedLit* floor = new TexturedLit(quad, "textures/marble.png");

	TransformNode* rootNode = new TransformNode(glm::mat4());
	LightNode* light = new LightNode(glm::vec3(20, 50, 50), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.4, 10, 200);
	//rootNode->AddChild(light);
	
	// create a cubemap
	std::vector<std::string> cubeMapFiles;
	cubeMapFiles.push_back("textures/cubemaps/miramar_up.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_dn.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_ft.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_bk.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_rt.png");
	cubeMapFiles.push_back("textures/cubemaps/miramar_lf.png");
	SOF::Cubemap* cubeMap = new SOF::Cubemap(cubeMapFiles);
	Skybox* sb = new Skybox(cubeMap);
	GeometryNode* sbNode = new GeometryNode(sb);

	light->AddChild(sbNode);

	// scale the floor and add 
	TransformNode* floorScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(100, 100, 100)));
	light->AddChild(floorScale);
	floorScale->AddChild(new GeometryNode(floor));
	
	SphereGeometry sphere(20);
	TexturedLit* sphereRender = new TexturedLit(sphere, "textures/stone.png", "default",cubeMap,0.3f);
	TransformNode* sphereTrans = new TransformNode(glm::translate(glm::mat4(), glm::vec3(0.0f, 10.0f, 0.0f)));
	TransformNode* sphereScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(3.0f, 3.0f, 3.0f)));

	light->AddChild(sphereTrans);
	sphereTrans->AddChild(sphereScale);
	sphereScale->AddChild(new GeometryNode(sphereRender));

	return light;
}
