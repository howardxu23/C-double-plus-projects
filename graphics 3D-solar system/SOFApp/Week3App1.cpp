#include "stdafx.h"
#include "week3app1.h"
#include "RenderLib.h"
#include <random>

void Week3App1::SetupLights()
{
	glm::mat4 lightPos;
	
	lightPos[0] = glm::vec4(-50.0f, -50.0f, 50.0f, 1.0f);
	lightPos[1] = glm::vec4(-50.0f, 50.0f, 50.0f, 1.0f);
	lightPos[2] = glm::vec4(50.0f, 50.0f, 50.0f, 1.0f);
	lightPos[3] = glm::vec4(50.0f, -50.0f, 50.0f, 1.0f);

	glm::mat4 lightColor;

	lightColor[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightColor[1] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightColor[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	lightColor[3] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	lights = PointLight(lightPos, lightColor, 0.2, 0.8, 0.5, 10, glm::vec4(50.0f, 50.0f, 50.0f, 50.0f));
}

void Week3App1::Init()
{
	QuadGeometry quad(10);
	floorQuad = new TexturedLit(quad, "textures/wood_diffuse.jpg", "textures/wood_normal.png");

	SetupLights();

	// setup the camera
	eye = glm::vec3(50.0f, 30.0f, 0.0f);
	lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0); // y is up!
	camera = new FlyCam(eye, lookAt);

#if TEST_CUBOID
	CuboidGeometry geom(5.0f, 10.0f, 2.0f);
	cuboid = new TexturedLit(geom, "textures/grid.jpg");
#else
	// construct the robot arm
	arm = new RobotArm();
#endif

	// render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Week3App1::Draw()
{
	// camera
	glm::mat4 view;
	glm::mat4 proj;

	eye = camera->GetPos();
	lookAt = camera->GetLookAt();
	up = camera->GetUp();
	view = glm::lookAt(eye, lookAt, up);
	proj = glm::perspective(glm::radians(60.0f), (float)SOF::Window::GetWidth() / (float)SOF::Window::GetHeight(), 0.1f, 1000.0f);

	Transforms trans(glm::mat4(), view, proj);
	// draw the floor
	glm::mat4 model = glm::scale(glm::mat4(), glm::vec3(100.0f,1.0f,100.0f));
	trans.SetModel(model);
	floorQuad->Draw(trans, lights);
	trans.SetModel(glm::mat4());
#if TEST_CUBOID
	cuboid->Draw(trans, lights);
#else
	arm->Draw(trans, lights);
#endif
}

void Week3App1::Update(float deltaT)
{
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

	if (SOF::Window::IsKeyDown(GLFW_KEY_F))//arm controls
		arm->RotateElement (0,20.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_V))
		arm->RotateElement(0, -20.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_G))
		arm->RotateElement(1, 20.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_B))
		arm->RotateElement(1, -20.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_H))
		arm->RotateElement(2, 20.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_N))
		arm->RotateElement(2, -20.0f * deltaT);
	
	//grabber controls
	if (SOF::Window::IsKeyDown(GLFW_KEY_J))
		arm->AddToGrabWidth(-1.0f * deltaT);
	if (SOF::Window::IsKeyDown(GLFW_KEY_M))
		arm->AddToGrabWidth(1.0f * deltaT);

}


