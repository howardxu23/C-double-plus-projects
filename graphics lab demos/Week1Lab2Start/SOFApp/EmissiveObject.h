#pragma once
#include "stdafx.h"
#include "ShaderProgram.h"
#include "Geometry.h"
#include "SphereGeom.h"
#include "Transforms.h"
class EmmisiveObject
{
	SOF::ShaderProgram* shader;
	SOF::Geometry* geom;

	void CreateGeometry( const GeometryHelper& geometry)
	{
		shader = new SOF::ShaderProgram("shaders/v_emissive.glsl", "shaders/f_emissive.glsl");
		SphereGeometry* vertices = new SphereGeometry(10); // low res sphere
		geom = new SOF::Geometry(vertices->GetNumVertices());
		geom->AddAttribute(geometry.GetPositionVector(), "vertexPos");
		geom->Finalize(shader);
		delete vertices;
	}
public:
	EmmisiveObject(const GeometryHelper &geometry) {CreateGeometry(geometry);}
	void Draw(const Transforms &trans, const glm::vec4& color)
	{
		glm::mat4 mvp;
		mvp = trans.GetProj() * trans.GetView() * trans.GetModel();
		shader->Use();
		shader->SetUniformMat4("mvp", mvp);
		shader->SetUniformVec4("color", color);
		geom->DrawPrimitives();
	}
};