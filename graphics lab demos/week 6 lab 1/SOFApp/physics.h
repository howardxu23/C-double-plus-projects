#pragma once
#include "stdafx.h"

struct ContactInfo
{
    glm::vec3 normal;
    float depth;
};
class StaticCollider
{
    // abstract interface for something that static that collides with a sphere
public:
    virtual bool Collide(const glm::vec3& r, float rad, glm::vec3& normal, float&depth) const = 0;
    virtual ~StaticCollider() {}
};
class QuadCollider : public StaticCollider
{
    glm::vec3 r, a, b;
    glm::vec3 aHat, bHat; // unit vectors along sides a and b
    float lena, lenb; // lengths of sides a and b
public:
    QuadCollider(const glm::vec3& r, const glm::vec3& a, const glm::vec3& b) : r(r), a(a), b(b)
    {
        lena = glm::length(a);
        lenb = glm::length(b);
        aHat = a / lena;
        bHat = b / lenb;
    }
    QuadCollider(glm::mat4 trans)
    {
        // this constructor assumes a unit quad centered on the origin with sides x, z
        // transform transforms this into place
        glm::vec3 rt = trans * glm::vec4(-0.5f, 0.0f, -0.5f, 1.0f);
        glm::vec3 at = trans * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 bt = trans * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
        r = glm::vec3(rt.x, rt.y, rt.z);
        a = glm::vec3(at.x, at.y, at.z);
        b = glm::vec3(bt.x, bt.y, bt.z);
        lena = glm::length(a);
        lenb = glm::length(b);
        aHat = a / lena;
        bHat = b / lenb;
    }
    ~QuadCollider() {}

    bool Collide(const glm::vec3& p, float rad, glm::vec3& normal, float& depth) const override
    {
        float la, lb;
        la = dot(p - r, aHat);
        lb = dot(p - r, bHat);
        if (la < 0.0f) la = 0.0f; if (la > lena) la = lena;
        if (lb < 0.0f) lb = 0.0f; if (lb > lenb) lb = lenb;
        glm::vec3 rNear=r + aHat * la + bHat * lb;
        
        if (glm::dot(p - rNear, p - rNear) < rad * rad)
        {
            normal = glm::normalize(p - rNear);
            depth = rad - glm::length(p - rNear);
            return true;
        }
        else
            return false;
    }
};
class Physics {
    std::vector<StaticCollider*> staticColliders;
public:
    void AddCollider(StaticCollider* coll) { staticColliders.push_back(coll); }
    std::vector<ContactInfo>CollideWithWorld(const glm::vec3& spherePos, float sphereRad) const;
};