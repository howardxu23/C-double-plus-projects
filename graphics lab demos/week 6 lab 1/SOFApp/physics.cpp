#include "stdafx.h"
#include "physics.h"
std::vector<ContactInfo> Physics::CollideWithWorld(const glm::vec3& spherePos, float sphereRad) const 
{
	std::vector<ContactInfo>contacts;

	bool anyColl = false;
	for (auto c : staticColliders) {
		glm::vec3 normal;
		float depth;
		if (c->Collide(spherePos, sphereRad,normal,depth))
		{
			contacts.push_back(ContactInfo{ normal,depth });
		}
	}
	return contacts;
}