#include "stdafx.h"
#include "week6app.h"

Node* Scene(Physics *phys);
class CamColl : public CameraCollisionCallback
{
	Physics* phys;
public:
	CamColl(Physics* phys) : phys(phys) {}
	glm::vec3 operator()(const glm::vec3& pos, const glm::vec3& newPos, float rad) const override
	{
		std::vector<ContactInfo>contacts = phys->CollideWithWorld(newPos, rad);
		glm::vec3 finalPos = newPos;
		
		for (auto c : contacts) {
			finalPos += c.normal * c.depth;
		}
		return finalPos;
	}
	virtual ~CamColl() {}
};
void Week6App::Init()
{
	// setup the camera
	eye = glm::vec3(0.0f, 3.0f, -40.0f);
	lookAt = glm::vec3(0.0f, 3.0f, 0.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0); // y is up!

	physics = new Physics();
	camera = new FlyCam(eye, lookAt, true,new CamColl(physics));
	sceneGraphRoot = Scene(physics);

	// scene graph for dynamic objects 
	dynamicObjectNode = new GroupNode();
	sceneGraphRoot->AddChild(dynamicObjectNode);
	/*
	// test cases for collision
	bool coll;
	coll = physics->CollideWithWorld(glm::vec3(0.0, 0.0, 0.0), 1.0f); std::cout << "case 1 " << std::boolalpha << coll << std::endl;
	coll = physics->CollideWithWorld(glm::vec3(0.0, 5.0, 0.0), 1.0f); std::cout << "case 2 " << std::boolalpha << coll << std::endl;
	coll = physics->CollideWithWorld(glm::vec3(50.0, 50.0, 0.0), 1.0f); std::cout << "case 3 " << std::boolalpha << coll << std::endl;
	coll = physics->CollideWithWorld(glm::vec3(-50.0, 0.0, 0.0), 1.0f); std::cout << "case 4 " << std::boolalpha << coll << std::endl;
	coll = physics->CollideWithWorld(glm::vec3(-50.0, 5.0, 0.0), 1.0f); std::cout << "case 5 " << std::boolalpha << coll << std::endl;
	coll = physics->CollideWithWorld(glm::vec3(-50.0, 50.0, 0.0), 0.0f); std::cout << "case 6 " << std::boolalpha << coll << std::endl;
	coll = physics->CollideWithWorld(glm::vec3(-50.0, 10.0, 60.0), 10.0f); std::cout << "case 7 " << std::boolalpha << coll << std::endl;
	*/
	//projectile
	projectileNode = new TransformNode(glm::scale(glm::mat4(), glm::vec3(0.2f, 0.2f, 0.2f)));
	SphereGeometry geom(20);
	TexturedLit* mesh = new TexturedLit(geom, "textures/grid.jpg");
	projectileNode->AddChild(new GeometryNode(mesh));
	// render states
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Week6App::Draw()
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
void Week6App::Shoot() {
	TransformNode* node = new TransformNode(glm::mat4());
	dynamicObjectNode->AddChild(node);
	node->AddChild(projectileNode);
	float speed = 20.0f;
	glm::vec3 vel=glm::normalize(camera->GetLookAt()-camera->GetPos())*speed;
	Projectile* proj = new Projectile(camera->GetPos(),vel,node);
	physics->AddProjectile(proj);
}
void Week6App::Update(float deltaT)
{
	physics->Update(deltaT);
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

	if (SOF::Window::IsKeyPressed(GLFW_KEY_SPACE)) {
		Shoot();
	}
}


