#include "stdafx.h"
#include "RenderLib.h"
#include <random>

Node* Example1()
{
	//
	// construct scenegraph example 1 and return a pointer to the root node 
	// this scene has a single light, floor quad, and some random geometry
	// the random geometry is made from a group which has a sphere, cube, and cylinder. 
	// this group is instanced multiple times in the scene graph (using the DAG structure).
	//
	QuadGeometry quad(10);
	TexturedLit* floor = new TexturedLit(quad, "textures/marble.png");

	TransformNode* rootNode = new TransformNode(glm::mat4());
	LightNode* light = new LightNode(glm::vec3(0, 50, 0), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.4, 10, 200);
	rootNode->AddChild(light);

	// scale the floor and add 
	light->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(100, 100, 100))));
	light->GetChild(0)->AddChild(new GeometryNode(floor));

	// make a group of geometry objects, and repeat it five times in random places and orientations
	CubeGeometry cube;
	SphereGeometry sphere(20);
	CylinderGeometry cylinder(1.0f,20);

	TexturedLit* cubeRender = new TexturedLit(cube, "textures/stone.png", "textures/stonenorms.png");
	TexturedLit* sphereRender = new TexturedLit(sphere, "textures/stone.png", "textures/stonenorms.png");
	TexturedLit* cylinderRender = new TexturedLit(cylinder, "textures/stone.png", "textures/stonenorms.png");

	GroupNode* group = new GroupNode();
	
	glm::mat4 trans;
	// cube transform - translation 10, 3, 5, rotation 45 about y, scale 3
	trans = glm::translate(glm::mat4(), glm::vec3(10, 3, 5));
	trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0, 1, 0));
	trans = glm::scale(trans, glm::vec3(3, 3, 3));
	group->AddChild(new TransformNode(trans));
	group->GetChild(0)->AddChild(new GeometryNode(cubeRender));

	// sphere transform - translation -5, 3, 8, scale 3
	trans = glm::translate(glm::mat4(), glm::vec3(-5, 3, 8));
	trans = glm::scale(trans, glm::vec3(3, 3, 3));
	group->AddChild(new TransformNode(trans));
	group->GetChild(1)->AddChild(new GeometryNode(sphereRender));

	// cylinder transform - translation 3, 3, -6, scale 3
	trans = glm::translate(glm::mat4(), glm::vec3(3, 3, -6));
	trans = glm::scale(trans, glm::vec3(3, 3, 3));
	group->AddChild(new TransformNode(trans));
	group->GetChild(2)->AddChild(new GeometryNode(cylinderRender));

	// make multiple copies of the group, each with a random translation
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist(-25.0f, 25.0f);

	for (int i = 0; i < 10; i++)
	{
		float x, y, z;
		x = dist(rng);
		y = 0.0f;
		z = dist(rng);
		TransformNode* t = new TransformNode(glm::translate(glm::mat4(), glm::vec3(x, y, z)));
		t->AddChild(group);
		light->AddChild(t);
	}

	return rootNode;
}

class MoveLightCallback : public UpdateCallback
{
	LightNode* node;
	float t;
public:
	MoveLightCallback(LightNode* node) : node(node), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		float x, y, z;
		x = 50.0f * cos(t);
		y = 20.0f;
		z = 50.0f * sin(t);
		node->light.position = glm::vec3(x, y, z);
	}
};

Node* Example2()
{
	// construct scenegraph example 2 and return a pointer to the root node 
	// this scene has just the floor, and an orbiting moving light

	QuadGeometry quad(10);
	TexturedLit* floor = new TexturedLit(quad, "textures/marble.png");

	TransformNode* rootNode = new TransformNode(glm::mat4());
	LightNode* light = new LightNode(glm::vec3(0, 50, 0), glm::vec4(1, 1, 1, 1), 0.1, 0.9, 0.4, 10, 50);
	rootNode->AddChild(light);

	light->SetUpdateCallback(new MoveLightCallback(light));
	// scale the floor and add 
	light->AddChild(new TransformNode(glm::scale(glm::mat4(), glm::vec3(100, 100, 100))));
	light->GetChild(0)->AddChild(new GeometryNode(floor));

	return rootNode;
}

//callback for animating a rotation about the y axis - use this for the solar system exercise
class YRotatorCallback : public UpdateCallback
{
	TransformNode* trans;
	float rate;
	float t;
public:
	YRotatorCallback( TransformNode *trans, float rate) : trans(trans), rate(rate), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		trans->SetTransform(glm::rotate(glm::mat4(), t * rate, glm::vec3(0, 1, 0)));
	}
};

Node* SolarSystem()
{
	// geometry and drawables
	SphereGeometry sphere(20);
	TexturedLit* earth = new TexturedLit(sphere, "textures/2k_Earth_daymap.jpg");
	TexturedLit* moon = new TexturedLit(sphere, "textures/2k_moon.jpg");
	Emissive* sun = new Emissive(sphere, glm::vec4(1.0f, 1.0f, 0.5f, 1.0f));

	// light node - use this as the root node
	LightNode* light = new LightNode(glm::vec3(0, 0, 0), glm::vec4(1, 1, 1, 1), 0.1, 1.0, 0.2, 10, 200);

	// TODO - construct the scenegraph for the animated solar system model
	TransformNode* sunScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)));
	TransformNode* earthScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(2, 2, 2)));
	TransformNode* earthOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* earthSpinRotator = new TransformNode(glm::mat4());
	TransformNode* earthTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(20, 0, 0)));
	TransformNode* moonTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(5, 0, 0)));
	TransformNode* moonOrbitRotator = new TransformNode(glm::mat4());
	//hook up the graph
	light->AddChild(sunScale);
	sunScale->AddChild(new GeometryNode(sun));

	light->AddChild(earthOrbitRotator);
	earthOrbitRotator->AddChild(earthTranslation);
	earthOrbitRotator->SetUpdateCallback(new YRotatorCallback(earthOrbitRotator, 1.0f));

	earthTranslation->AddChild(moonOrbitRotator);
	moonOrbitRotator->AddChild(moonTranslation);
	moonOrbitRotator->SetUpdateCallback(new YRotatorCallback(moonOrbitRotator, 5.0f));
	moonTranslation->AddChild(new GeometryNode(moon));

	earthTranslation->AddChild(earthSpinRotator);
	earthSpinRotator->AddChild(earthScale);
	earthSpinRotator->SetUpdateCallback(new YRotatorCallback(earthSpinRotator, 5.0f));
	earthScale->AddChild(new GeometryNode(earth));
	return light;
}