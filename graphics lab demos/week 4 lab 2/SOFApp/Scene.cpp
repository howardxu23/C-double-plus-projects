#include "stdafx.h"
#include "RenderLib.h"
#include "spline.h"
#include <random>

std::vector<glm::vec3> splinePoints;

// callback for animating on a spline
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

	TransformNode* sphereTrans = new TransformNode(glm::mat4());
	light->AddChild(sphereTrans);
	sphereTrans->AddChild(sphereNode);
	sphereTrans->SetUpdateCallback(new SplineAnimationCallback(sphereTrans, new CRspline(splinePoints, true, 0.5f), 1.0f));

	return light;
}
