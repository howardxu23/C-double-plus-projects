#include "stdafx.h"
#include "week1app.h"
#include "Transforms.h"
#include "CubeGeom.h"
#include <random>

void Week1App::Init()
{
	SphereGeometry sphereGeom(20);
	CubeGeometry cubegeom;
	sphere = new TexturedLitObject(cubegeom, "textures/wood_diffuse.jpg", "textures/wood_normal.png");
	lightSphere = new EmmisiveObject(sphereGeom);

	// use random number generator to place the spheres
	std::mt19937 rng = std::mt19937(std::random_device()()); // mersenne twister RNG seeded from random source
	std::uniform_real_distribution<float> dist(0.0f, 1.0f); // uniform [0,1] random number distribution
	for (int i = 0; i < numSpheres; i++)
	{
		float x = -100.0f + 200.0f * dist(rng);
		float z = -100.0f + 200.0f * dist(rng);
		float y = 50.0f * dist(rng);
		float radius = 1.0f + 5.0f * dist(rng);
		spherePositions[i] = glm::vec4(x, y, z, radius);
	}
	lightPos = glm::vec3(0.0f, 25.0f, 0.0f); // in the middle of the cloud of spheres
	// render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Week1App::Draw()
{
	// camera
	glm::vec3 eye = glm::vec3(0.0f, 60.0f, -100.0f);
	glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0); // y is up!

	glm::mat4 view;
	glm::mat4 proj;
	view = glm::lookAt(eye, lookAt, up);
	proj = glm::perspective(glm::radians(60.0f), (float)SOF::Window::GetWidth() / (float)SOF::Window::GetHeight(), 0.1f, 1000.0f);

	glm::mat4 model = glm::mat4();
	Transforms trans(model, view, proj);

	// light object - color white, ambient 0.2, diffuse 1.0, specular 0.5, spec power 3, falloff radius 50.0
	PointLight light(lightPos, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.2f, 1.0f, 0.5f, 3.0f, 50.0f);
	// draw the spheres
	for (int i = 0; i < numSpheres; i++)
	{
		model = glm::translate(glm::mat4(), glm::vec3(spherePositions[i]));
		float scale = spherePositions[i].w;
		model = glm::scale(model, glm::vec3(scale, scale, scale));
		trans.SetModel(model);
		sphere->Draw(trans,light);
	}
	// draw the light
	model = glm::translate(glm::mat4(), lightPos);
	trans.SetModel(model);
	lightSphere->Draw(trans, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void Week1App::Update(float deltaT)
{
	// move the light around
	float speed = 20.0f;
	if (SOF::Window::IsKeyDown(GLFW_KEY_A))
		lightPos.x += speed*deltaT;
	if (SOF::Window::IsKeyDown(GLFW_KEY_D))
		lightPos.x -= speed*deltaT;
	if (SOF::Window::IsKeyDown(GLFW_KEY_W))
		lightPos.z += speed*deltaT;
	if (SOF::Window::IsKeyDown(GLFW_KEY_S))
		lightPos.z -= speed*deltaT;
}	


