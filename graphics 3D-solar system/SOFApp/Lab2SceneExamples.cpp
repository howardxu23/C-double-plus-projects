#include "stdafx.h"
#include "RenderLib.h"
#include <random>
#include "spline.h"
#include "stdlib.h"
#include "physics.h"
std::vector<glm::vec3> splinePoints;

class SplineAnimationCallback : public UpdateCallback
{
	Spline* spline;
	TransformNode* trans;
	float rate;
	float t = 0;
public:
	SplineAnimationCallback(TransformNode* trans, Spline* spline, float rate) : trans(trans), spline(spline), rate(rate), t(0.0f) {}
	virtual void operator()(float dt)
	{
		t += dt;
		if (t > spline->GetNumPoints())
			t -= spline->GetNumPoints();
		// work out the new transform here
		glm::vec3 position = spline->GetPoint(t);
		glm::vec3 tangent = spline->GetTangent(t);
		glm::mat4 lookAt = glm::lookAt(position, position + tangent, glm::vec3(0, 1, 0));
		trans->SetTransform(glm::inverse(lookAt));
	}
};

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

Node* SolarSystem(Physics* phys){
	// geometry and drawables
	SphereGeometry sphere(20);
	QuadGeometry quad(10);

	//assigns textures
	TexturedLit* earth = new TexturedLit(sphere, "textures/8k_earth_daymap.jpg","textures/8k_earth_normal_map.jpg");
	TexturedLit* moon = new TexturedLit(sphere, "textures/2k_moon.jpg");
	Emissive* sun = new Emissive(sphere, glm::vec4(1.0f, 1.0f, 0.5f, 10.0f));
	TexturedLit* mecury = new TexturedLit(sphere, "textures/8k_mercury.jpg");
	TexturedLit* venus = new TexturedLit(sphere, "textures/2k_venus_atmosphere.jpg");
	TexturedLit* mars = new TexturedLit(sphere, "textures/2k_mars.jpg");
	TexturedLit* jupiter = new TexturedLit(sphere, "textures/2k_jupiter.jpg");
	TexturedLit* saturn = new TexturedLit(sphere, "textures/2k_saturn.jpg");
	TexturedLit* uranus = new TexturedLit(sphere, "textures/2k_uranus.jpg");
	TexturedLit* neptune = new TexturedLit(sphere, "textures/2k_neptune.jpg");
	TexturedLit* floor = new TexturedLit(quad, "textures/marble.png");
	// light node - use this as the root node
	LightNode* light = new LightNode(glm::vec3(0, 0, 0), glm::vec4(1, 1, 1, 1), 0.1, 1.0, 0.2, 10, 200);

	// TODO - construct the scenegraph for the animated solar system model
	//set up  the scale, orbits, their position and rotation
	TransformNode* sunScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(30, 30, 30)));
	
	//mecury
	TransformNode* mecuryScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(2, 2, 2)));
	TransformNode* mecuryOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* mecurySpinRotator = new TransformNode(glm::mat4());
	TransformNode* mecuryTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(35, 0, 0)));
	TransformNode* mecuryRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	//venus
	TransformNode* venusScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)));
	TransformNode* venusOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* venusSpinRotator = new TransformNode(glm::mat4());
	TransformNode* venusTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(55, 0, 0)));
	TransformNode* venusRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	//earth
	TransformNode* earthScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(5, 5, 5)));
	TransformNode* earthOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* earthSpinRotator = new TransformNode(glm::mat4());
	TransformNode* earthTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(80, 0, 0)));
	TransformNode* earthRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0,0)));
	//moon
	TransformNode* moonScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(2, 2, 2)));
	TransformNode* moonTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(15, 0, 0)));
	TransformNode* moonRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	TransformNode* moonOrbitRotator = new TransformNode(glm::mat4());
	//mars
	TransformNode* marsScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(3, 3, 3)));
	TransformNode* marsOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* marsSpinRotator = new TransformNode(glm::mat4());
	TransformNode* marsTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(120, 0, 0)));
	TransformNode* marsRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	//jupiter
	TransformNode* jupiterScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(20, 20, 20)));
	TransformNode* jupiterOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* jupiterSpinRotator = new TransformNode(glm::mat4());
	TransformNode* jupiterTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(190, 0, 0)));
	TransformNode* jupiterRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	//saturn
	TransformNode* saturnScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(18, 18, 18)));
	TransformNode* saturnOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* saturnSpinRotator = new TransformNode(glm::mat4());
	TransformNode* saturnTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(240, 0, 0)));
	TransformNode* saturnRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	//uranus
	TransformNode* uranusScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(10, 10, 10)));
	TransformNode* uranusOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* uranusSpinRotator = new TransformNode(glm::mat4());
	TransformNode* uranusTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(280, 0, 0)));
	TransformNode* uranusRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0)));
	//neptune
	TransformNode* neptuneScale = new TransformNode(glm::scale(glm::mat4(), glm::vec3(10, 10, 10)));
	TransformNode* neptuneOrbitRotator = new TransformNode(glm::mat4());
	TransformNode* neptuneSpinRotator = new TransformNode(glm::mat4());
	TransformNode* neptuneTranslation = new TransformNode(glm::translate(glm::mat4(), glm::vec3(320, 0, 0)));
	TransformNode* neptuneRotation = new TransformNode(glm::rotate(glm::mat4(), glm::radians(180.0f), glm::vec3(1, 0, 0)));

	//hook up the graphs
	//create skybox
	std::vector<std::string> cubeMapFiles;
	cubeMapFiles.push_back("textures/cubemaps/2k_stars.jpg");
	cubeMapFiles.push_back("textures/cubemaps/2k_stars.jpg");
	cubeMapFiles.push_back("textures/cubemaps/2k_stars.jpg");
	cubeMapFiles.push_back("textures/cubemaps/2k_stars.jpg");
	cubeMapFiles.push_back("textures/cubemaps/2k_stars.jpg");
	cubeMapFiles.push_back("textures/cubemaps/2k_stars.jpg");

	SOF::Cubemap* cubeMap = new SOF::Cubemap(cubeMapFiles);
	Skybox* sb = new Skybox(cubeMap);
	GeometryNode* sbNode = new GeometryNode(sb);
	light->AddChild(sbNode);//creates skybox

	//sun
	light->AddChild(sunScale);
	sunScale->AddChild(new GeometryNode(sun));
	
	//mecury
	light->AddChild(mecuryOrbitRotator);
	mecuryOrbitRotator->AddChild(mecuryTranslation);
	mecuryOrbitRotator->SetUpdateCallback(new YRotatorCallback(mecuryOrbitRotator, 1.0f));
	mecuryTranslation->AddChild(mecurySpinRotator);
	mecurySpinRotator->AddChild(mecuryRotation);
	mecurySpinRotator->SetUpdateCallback(new YRotatorCallback(mecurySpinRotator, 0.2f));
	mecuryRotation->AddChild(mecuryScale);
	mecuryScale->AddChild(new GeometryNode(mecury));
	//venus
	light->AddChild(venusOrbitRotator);
	venusOrbitRotator->AddChild(venusTranslation);
	venusOrbitRotator->SetUpdateCallback(new YRotatorCallback(venusOrbitRotator, 0.9f));
	venusTranslation->AddChild(venusSpinRotator);
	venusSpinRotator->AddChild(venusRotation);
	venusSpinRotator->SetUpdateCallback(new YRotatorCallback(venusSpinRotator, 0.2f));
	venusRotation->AddChild(venusScale);
	venusScale->AddChild(new GeometryNode(venus));
	//earth
	light->AddChild(earthOrbitRotator);
	earthOrbitRotator->AddChild(earthTranslation);
	earthOrbitRotator->SetUpdateCallback(new YRotatorCallback(earthOrbitRotator, 0.5f));
	earthTranslation->AddChild(moonOrbitRotator);//attaches the moon to the earth object
	earthTranslation->AddChild(earthSpinRotator);
	earthSpinRotator->AddChild(earthScale);
	earthScale->AddChild(earthRotation);
	earthSpinRotator->SetUpdateCallback(new YRotatorCallback(earthSpinRotator, 2.0f));
	earthRotation->AddChild(new GeometryNode(earth));
	//moon
	moonOrbitRotator->AddChild(moonTranslation);
	moonOrbitRotator->SetUpdateCallback(new YRotatorCallback(moonOrbitRotator, 1.2f));
	moonTranslation->AddChild(moonScale);
	moonScale->AddChild(moonRotation);
	moonRotation->AddChild(new GeometryNode(moon));
	//mars
	light->AddChild(marsOrbitRotator);
	marsOrbitRotator->AddChild(marsTranslation);
	marsOrbitRotator->SetUpdateCallback(new YRotatorCallback(marsOrbitRotator, 0.4f));
	marsTranslation->AddChild(marsSpinRotator);
	marsSpinRotator->AddChild(marsScale);
	marsScale->AddChild(marsRotation);
	marsSpinRotator->SetUpdateCallback(new YRotatorCallback(marsSpinRotator, 2.0f));
	marsRotation->AddChild(new GeometryNode(mars));
	//jupiter
	light->AddChild(jupiterOrbitRotator);
	jupiterOrbitRotator->AddChild(jupiterTranslation);
	jupiterOrbitRotator->SetUpdateCallback(new YRotatorCallback(jupiterOrbitRotator, 0.4f));
	jupiterTranslation->AddChild(jupiterSpinRotator);
	jupiterSpinRotator->AddChild(jupiterScale);
	jupiterScale->AddChild(jupiterRotation);
	jupiterSpinRotator->SetUpdateCallback(new YRotatorCallback(jupiterSpinRotator, 0.5f));
	jupiterRotation->AddChild(new GeometryNode(jupiter));
	//saturn
	light->AddChild(saturnOrbitRotator);
	saturnOrbitRotator->AddChild(saturnTranslation);
	saturnOrbitRotator->SetUpdateCallback(new YRotatorCallback(saturnOrbitRotator, 0.3f));
	saturnTranslation->AddChild(saturnSpinRotator);
	saturnSpinRotator->AddChild(saturnScale);
	saturnScale->AddChild(saturnRotation);
	saturnSpinRotator->SetUpdateCallback(new YRotatorCallback(saturnSpinRotator, 0.5f));
	saturnRotation->AddChild(new GeometryNode(saturn));
	//uranus
	light->AddChild(uranusOrbitRotator);
	uranusOrbitRotator->AddChild(uranusTranslation);
	uranusOrbitRotator->SetUpdateCallback(new YRotatorCallback(uranusOrbitRotator, 0.2f));
	uranusTranslation->AddChild(uranusSpinRotator);
	uranusSpinRotator->AddChild(uranusScale);
	uranusScale->AddChild(uranusRotation);
	uranusSpinRotator->SetUpdateCallback(new YRotatorCallback(uranusSpinRotator, 0.5f));
	uranusRotation->AddChild(new GeometryNode(uranus));
	//neptune
	light->AddChild(neptuneOrbitRotator);
	neptuneOrbitRotator->AddChild(neptuneTranslation);
	neptuneOrbitRotator->SetUpdateCallback(new YRotatorCallback(neptuneOrbitRotator, 0.1f));
	neptuneTranslation->AddChild(neptuneSpinRotator);
	neptuneSpinRotator->AddChild(neptuneScale);
	neptuneScale->AddChild(neptuneRotation);
	neptuneSpinRotator->SetUpdateCallback(new YRotatorCallback(neptuneSpinRotator, 0.5f));
	neptuneRotation->AddChild(new GeometryNode(neptune));
	
	//makes spline points with random values
	srand(time(NULL));//seeds the time

	splinePoints.push_back(glm::vec3(50+rand() % (30 - -10 + 1) + 10, 1, rand() % (30 - -10 + 1) + 10));
	splinePoints.push_back(glm::vec3(50+rand() % (50 - 6 + 1) + 6, 1, -(rand() % (50 - 20 + 1) + 6)));
	splinePoints.push_back(glm::vec3(50+rand() % (24 - -24 + 1) + -24, 1, -(rand() % (50 - 30 + 1) + 30)));
	splinePoints.push_back(glm::vec3(50+rand() % (24 - -24 + 1) + -24, 1, (rand() % (40 - 30 + 1) + 30)));
	splinePoints.push_back(glm::vec3(50+rand() % (50 - 6 + 1) + 6, 1, -(rand() % (50 - 20 + 1) + 6)));
	splinePoints.push_back(glm::vec3(50 + rand() % (30 - 6 + 1) + 6, 1, -(rand() % (50 - 20 + 1) + 6)));
	splinePoints.push_back(glm::vec3(50 + -( rand() % (24 - -24 + 1) + -24), 1, -(rand() % -(50 - 30 + 1) + 30)));

	TexturedLit* sphereRender = new TexturedLit(sphere, "textures/marble.png", "textures/default_normal.png",cubeMap, 0.7f);
	GeometryNode* sphereNode = new GeometryNode(sphereRender);
	for (glm::vec3 pos : splinePoints)
	{
		TransformNode* sphereTrans = new TransformNode(glm::translate(glm::mat4(), pos));
		light->AddChild(sphereTrans);
		sphereTrans->AddChild(sphereNode);
	}
	//makes line connecting the spline points
	std::vector<glm::vec3> lineVerts;
	CRspline c(splinePoints, true, 0.5f);
	for (int i = 0; i < 500; i++)
	{
		float u = 8.0f * i / 499.0f;
		lineVerts.push_back(c.GetPoint(u));
	}
	PolyLine* p = new PolyLine(lineVerts, glm::vec4(1, 0, 0, 1));
	light->AddChild(new GeometryNode(p));
	
	//adds a sphere that is animated and moves along the spline line
	TransformNode* sphereTrans = new TransformNode(glm::mat4());
	light->AddChild(sphereTrans);
	sphereTrans->AddChild(sphereNode);
	sphereTrans->SetUpdateCallback(new SplineAnimationCallback(sphereTrans, new CRspline(splinePoints, true, 0.5f), 1.0f));

	// scale the floor and add 
	GeometryNode* geomNode = new GeometryNode(floor);
	// scale the floor and add 
	glm::mat4 floorMat=glm::translate(glm::mat4(), glm::vec3(0, -40, -40));
	floorMat = glm::scale(floorMat, glm::vec3(100, 100, 100));

	TransformNode* floorScale = new TransformNode(floorMat);
	light->AddChild(floorScale);
	floorScale->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(floorMat));

	// make a wall
	glm::mat4 wtrans = glm::mat4();
	wtrans = glm::translate(wtrans, glm::vec3(50.0, -40.0, -40.0));
	wtrans = glm::rotate(wtrans, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0));
	wtrans = glm::scale(wtrans, glm::vec3(20.0f, 20.0f, 100.0f));
	TransformNode* wall0 = new TransformNode(wtrans);

	light->AddChild(wall0);
	wall0->AddChild(geomNode);
	phys->AddCollider(new QuadCollider(wtrans));
	return light;
}