#include "stdafx.h"
#include "RobotArm.h"

void RobotArm::UpdateTransforms(const glm::mat4& startModel )
{
	// update transforms here
	glm::mat4 model = startModel;
	model = glm::rotate(model, glm::radians( angles[0]), glm::vec3(0, 1, 0));
	transforms[0] = model;

	model = glm::translate(model, glm::vec3(0, h[0], 0));
	model = glm::rotate(model, glm::radians(angles[1]), glm::vec3(0, 0, 1));
	transforms[1] = model;

	model = glm::translate(model, glm::vec3(0, h[1], 0));
	model = glm::rotate(model, glm::radians(angles[2]), glm::vec3(0, 0, 1));
	transforms[2] = model;
	//grabber
	model = glm::translate(model, glm::vec3(0, h[2], 0));
	transforms[3] = model;

	model = glm::translate(model, glm::vec3(0, h[3], 0));
	transforms[4] = glm::translate(model, glm::vec3(0, 0, grabWidth));
	transforms[5] = glm::translate(model, glm::vec3(0, 0, -grabWidth));
}

void RobotArm::Draw(const Transforms& trans, const PointLight& light)
{
	Transforms t = trans; // copy to modify
	UpdateTransforms(t.GetModel());
	// render the meshes here
	for(int i = 0; i < 6; i++) {
		t.SetModel(transforms[i]);
		meshes[i]->Draw(t, light);
	}
}