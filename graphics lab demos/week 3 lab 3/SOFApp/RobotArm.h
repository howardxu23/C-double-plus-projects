#pragma once
#include "Transforms.h"
#include "PointLight.h"
#include "CuboidGeom.h"
#include "TexturedLit.h"
#include <vector>

class RobotArm
{
	// three part robot arm, see week 5 lecture
	float angles[3]; // three angles which define the state
	float h[6];	   // heights (y) of the components
	float w[6];	   // widths (x) of the components
	float l[6];    // lengths (z) of the components
	float grabWidth =1.0f;//width of jaws
	TexturedLit* meshes[6];
	glm::mat4 transforms[6];

public:
	RobotArm()
	{
		// main arm
		h[0] = 4.0f; w[0] = 4.0f; l[0] = 4.0f;
		h[1] = 8.0f; w[1] = 1.5f; l[1] = 1.5f;
		h[2] = 8.0f; w[2] = 1.5f; l[2] = 1.5f;
		//grabber
		h[3] = 1.0f; w[3] = 1.0f; l[3] = 5.0f;
		h[4] = 3.0f; w[4] = 0.75f; l[4] = 0.75f;
		h[5] = 3.0f; w[5] = 0.75f; l[5] = 0.75f;

		angles[0] = angles[1] = angles[2] = 0.0f;
		// set the three transform to the identity matrix
		for (int i = 0; i < 6; i++)
			transforms[i] = glm::mat4();
		// create the geometry
		for (int i = 0; i < 6; i++)
		{
			CuboidGeometry c(w[i], h[i], l[i]);
			meshes[i] = new TexturedLit(c, "textures/grid.jpg");
		}
	}

	void RotateElement(int i, float delta ) { angles[i] += delta; }

	void AddToGrabWidth(float delta) {
		grabWidth += delta;
		if (grabWidth < 0.75f/2) {
			grabWidth = 0.75f/2;
		}
		if (grabWidth > 2.5f) {
			grabWidth = 2.5f;
		}
	}
	void UpdateTransforms( const glm::mat4 & model);
	void Draw(const Transforms& t, const PointLight& light);

};