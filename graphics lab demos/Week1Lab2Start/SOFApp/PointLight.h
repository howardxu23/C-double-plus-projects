#pragma once
#include "stdafx.h"
// data class to hold lighting information, for ease of passing around the system

class PointLight
{
	glm::vec3 position;
	glm::vec4 color;
	float ambientLevel;
	float diffuseLevel;
	float specularLevel;
	float specularPower;
	float attenuationRadius;
public:
	PointLight(const glm::vec3& position, const glm::vec4& color, float ambientLevel,
		float diffuseLevel, float specularLevel, float specularPower, float attenuationRadius) :
		position(position), color(color), ambientLevel(ambientLevel), diffuseLevel(diffuseLevel),
		specularLevel(specularLevel), specularPower(specularPower), attenuationRadius(attenuationRadius) {}
	// getters
	const glm::vec3& GetPosition() const { return position;  }
	const glm::vec4& GetColor() const { return color; }
	float GetAmbientLevel() const { return ambientLevel; }
	float GetDiffuseLevel() const { return diffuseLevel; }
	float GetSpecularLevel() const { return specularLevel; }
	float GetSpecularPower() const { return specularPower; }
	float GetAttenuationRadius() const { return attenuationRadius; }
	// setters
	void SetPosition(const glm::vec3 &newPos) { position = newPos; }
	void SetColor(const glm::vec4 & newColor) { color = newColor; }
	void SetAmbientLevel(float newAmbient) { ambientLevel = newAmbient; }
	void SetDiffuseLevel(float newDiffuse) { diffuseLevel = newDiffuse; }
	void SetSpecularLevel(float newSpecular) { specularLevel = newSpecular; }
	void SetSpecularPower(float newPower) { specularPower = newPower; }
};