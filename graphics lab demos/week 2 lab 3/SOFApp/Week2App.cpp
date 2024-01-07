#include "stdafx.h"
#include "week2app.h"
#include "Transforms.h"
#include "CubeGeom.h"
#include"QuadGeom.h"
#include "CylinderGeom.h"
#include <random>

void Week2App::Init()
{
	mesh = new MeshObject("meshes/sheep/statuette.fbx");
	SphereGeometry sphereGeom(20);
	CubeGeometry cubeGeom;
	object = new TexturedLit(cubeGeom , "textures/wood_diffuse.jpg", "textures/wood_normal.png");
	lightSphere = new Emissive( sphereGeom );
	camera = new FlyCam(glm::vec3(0.0f, 60.0f, -100.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	CylinderGeometry cyl(1.0f, 16);
	cylinder = new TexturedLit(cyl, "textures/grid.jpg");
	//floor
	QuadGeometry quad(10);
	floorQuad = new TexturedLit(quad, "textures/wood_diffuse.jpg", "textures/wood_normal.png");

	// use random number generator to place the spheres
	std::mt19937 rng = std::mt19937(std::random_device()()); // mersenne twister RNG seeded from random source
	std::uniform_real_distribution<float> dist(0.0f, 1.0f); // uniform [0,1] random number distribution
	for (int i = 0; i < numObjects; i++)
	{
		float x = -100.0f + 200.0f * dist(rng);
		float z = -100.0f + 200.0f * dist(rng);
		float y = 50.0f * dist(rng);
		float radius = 1.0f + 5.0f * dist(rng);
		objectPositions[i] = glm::vec4(x, y, z, radius);
	}
	// fixed lights
	lightColors[0] = glm::vec4(0.5f, 0.6f, 1.0f, 1.0f);
	lightColors[1] = glm::vec4(0.8f, 0.6f, 1.0f, 1.0f);
	lightColors[2] = glm::vec4(0.8f, 0.3f, 1.0f, 1.0f);
	lightColors[3] = glm::vec4(0.5f, 0.8f, 1.0f, 1.0f);

	lightPos[0] = glm::vec3(-100.0f, 50.0f, 100.0f);
	lightPos[1] = glm::vec3(100.0f, 50.0f, 100.0f);
	lightPos[2] = glm::vec3(100.0f, 50.0f, -100.0f);
	lightPos[3] = glm::vec3(-100.0f, 50.0f, -100.0f);

	lightRadius[0] = lightRadius[1] = lightRadius[2] = lightRadius[3] = 200.0f;
	/*
	for (int i = 0; i < 4; i++)
		lightPos[i] = glm::vec3(0.0f, 25.0f, 0.0f);*/
	// render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Week2App::Draw()
{
	// camera
	glm::vec3 eye = camera->GetPos();
	glm::vec3 lookAt = camera->GetLookAt();
	glm::vec3 up = camera->GetUp();

	glm::mat4 view;
	glm::mat4 proj;
	view = glm::lookAt(eye, lookAt, up);
	proj = glm::perspective(glm::radians(60.0f), (float)SOF::Window::GetWidth() / (float)SOF::Window::GetHeight(), 0.1f, 1000.0f);

	glm::mat4 model = glm::mat4();
	Transforms trans(model, view, proj);

	// light object - color white, ambient 0.2, diffuse 1.0, specular 0.5, spec power 3, falloff radius 50.0
	glm::mat4 lightPositionMat;
	glm::mat4 lightColorMat;

	for (int i = 0; i < 4; i++)
	{
		lightPositionMat[i] = glm::vec4(lightPos[i], 0.0f);
		lightColorMat[i] = lightColors[i];
	}

	PointLight light(lightPositionMat, lightColorMat, 0.1f, 0.8f, 0.5f, 20, glm::vec4(lightRadius[0], lightRadius[1], lightRadius[2], lightRadius[3]));
#if 0
	// draw the objects
	for (int i = 0; i < numObjects; i++)
	{
		model = glm::translate(glm::mat4(), glm::vec3(objectPositions[i]));
		float scale = objectPositions[i].w;
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		trans.SetModel(model);
		object->Draw(trans, light);
	}
#else
	/*
	model = glm::mat4();
	trans.SetModel(model);
	mesh->Draw(trans, light);
	*/
	cylinder->Draw(trans, light);
	// draw the floor
	/*
	model = glm::scale(glm::mat4(), glm::vec3(100.0f, 1.0f, 100.0f));
	trans.SetModel(model);
	floorQuad->Draw(trans, light);*/
#endif
    // draw the lights
	for (int i = 0; i < 4; i++)
	{
		model = glm::translate(glm::mat4(), lightPos[i]);
		trans.SetModel(model);
		lightSphere->Draw(trans, lightColors[i]);
	}
}

void Week2App::Update(float deltaT)
{
	/*
	// move the lights around
	float speed = 20.0f;

	for (int i = 0; i < 4; i++)
	{
		if (SOF::Window::IsKeyDown(GLFW_KEY_1 + i))
		{
			if (SOF::Window::IsKeyDown(GLFW_KEY_A))
				lightPos[i].x += speed * deltaT;
			if (SOF::Window::IsKeyDown(GLFW_KEY_D))
				lightPos[i].x -= speed * deltaT;
			if (SOF::Window::IsKeyDown(GLFW_KEY_W))
				lightPos[i].z += speed * deltaT;
			if (SOF::Window::IsKeyDown(GLFW_KEY_S))
				lightPos[i].z -= speed * deltaT;
		}
	}*/
	const float panspeed = 90.0f;
	const float movespeed = 20;

	float move = 0.0f;
	float strafe = 0.0f;
	float pan = 0;
	float updown = 0;
	//pan
	if (SOF::Window::IsKeyDown(GLFW_KEY_LEFT)) {
		pan += panspeed * deltaT;
	}
	if (SOF::Window::IsKeyDown(GLFW_KEY_RIGHT))
		pan -= panspeed * deltaT;
	if (SOF::Window::IsKeyDown(GLFW_KEY_UP))
		updown += panspeed * deltaT;
	if (SOF::Window::IsKeyDown(GLFW_KEY_DOWN))
		updown -= panspeed * deltaT;
	//move
	
		if (SOF::Window::IsKeyDown(GLFW_KEY_A))
			strafe -= movespeed * deltaT;
		if (SOF::Window::IsKeyDown(GLFW_KEY_D))
			strafe += movespeed * deltaT;
		if (SOF::Window::IsKeyDown(GLFW_KEY_W))
			move += movespeed * deltaT;
		if (SOF::Window::IsKeyDown(GLFW_KEY_S))
			move -= movespeed * deltaT;
	camera->Move(move);
	camera->Strafe(strafe);
	camera->Pan(pan);
	camera->LookUpDown(updown);
}
