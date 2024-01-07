#include "stdafx.h"
#include "week3app3.h"

extern Node* Scene();

void Week3App3::Init()
{
	// setup the camera
	eye = glm::vec3(0.0f, 30.0f, -50.0f);
	lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0); // y is up!
	camera = new FlyCam(eye, lookAt);

	sceneGraphRoot = Scene();
	// render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Week3App3::Draw()
{
	// camera
	glm::mat4 view;
	glm::mat4 proj;

	eye = camera->GetPos();
	lookAt = camera->GetLookAt();
	up = camera->GetUp();
	view = glm::lookAt(eye, lookAt, up);
	proj = glm::perspective(glm::radians(60.0f), (float)SOF::Window::GetWidth() / (float)SOF::Window::GetHeight(), 0.1f, 1000.0f);
	// Render the scene graph
	RenderVisitor rv(view, proj);
	rv.Traverse(sceneGraphRoot);
}

void Week3App3::Update(float deltaT)
{
	// Upadate the scene graph
	UpdateVisitor uv(deltaT);
	uv.Traverse(sceneGraphRoot);

	if (SOF::Window::IsKeyDown(GLFW_KEY_W))
		camera->Move(10.0f*deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_S))
		camera->Move(-10.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_A))
		camera->Strafe(-10.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_D))
		camera->Strafe(10.0f * deltaT);

	if (SOF::Window::IsKeyDown(GLFW_KEY_LEFT))
		camera->Pan(40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_RIGHT))
		camera->Pan(-40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_UP))
		camera->LookUpDown(40.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_DOWN))
		camera->LookUpDown(-40.0f * deltaT);
}


