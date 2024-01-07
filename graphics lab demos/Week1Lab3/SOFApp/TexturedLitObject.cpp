#include "stdafx.h"
#include "TexturedLitObject.h"
#include "spheregeom.h"
#include <cmath>
#include "PointLight.h"
void TexturedLitObject::CreateGeometry(const GeometryHelper& geometry)
{
	// use the helper to calculate geometry
	//SphereGeometry *vertices = new SphereGeometry(tesselation);
	// load the shader program
	shader = new SOF::ShaderProgram("shaders/vshader.glsl", "shaders/fshader.glsl");
	
	// make the SOF::Geometry object
	geom = new SOF::Geometry(geometry.GetNumVertices());
	// add the position attribute
	geom->AddAttribute(geometry.GetPositionVector(), "vertexPos");
	// add the noermal attribute
	geom->AddAttribute(geometry.GetNormalVector(), "vertexNormal");
    //ADD UV cood atribute
	geom->AddAttribute(geometry.GetUVVector(), "vertexUV");

	geom->AddAttribute(geometry.GetTangentVector(), "vertexTangent");
	// finish up
	geom->Finalize(shader);
	// clean up
	//delete vertices;
}

void TexturedLitObject::Draw(const Transforms &trans, const PointLight& light)
{
	// calculate the mvp matrix;
	glm::mat4 vp = trans.GetProj() * trans.GetView();

	

	// enable the shader, set uniforms
	shader->Use();
	shader->SetUniformMat4("viewProj", vp);
	shader->SetUniformMat4("model", trans.GetModel());
	
	shader->SetUniformVec3("lightK",glm::vec3(light.GetAmbientLevel(), light.GetDiffuseLevel(), light.GetSpecularLevel()));
	shader->SetUniformFloat("alpha", light.GetSpecularPower());
	shader->SetUniformVec3("lightPos", light.GetPosition());
	shader->SetUniformVec4("ambientColor", light.GetColor());
	shader->SetUniformVec4("lightColor", light.GetColor());
	float attenuation = 1.0f / (light.GetAttenuationRadius() * light.GetAttenuationRadius());
	shader->SetUniformFloat("attenuation", attenuation);

	glm::vec3 camPos = -glm::mat3(trans.GetView()) * glm::vec3(trans.GetView()[3]);
	shader->SetUniformVec3("cameraPos", camPos);
	// hook up the texture 
	shader->AssignTextureSampler("textureSampler", texture);
	//normal map
	shader->AssignTextureSampler("normalSampler", normalMap);
	// draw the triangles
	geom->DrawPrimitives();
}
