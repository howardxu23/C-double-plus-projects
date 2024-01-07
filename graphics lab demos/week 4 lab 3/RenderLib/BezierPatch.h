#pragma once
#include "GeometryHelper.h"

class BezierPatch : public GeometryHelper
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> tangents;
    std::vector<glm::vec2> uvs;

    float BernsteinPoly(int i, float x) const
    {
        // evaluates the ith cubic Bernstein polynomial at value x
        float val = 1.0f;
        switch (i)
        {
        case 0:
            val = (1.0f - x) * (1.0f - x) * (1.0f - x); break;
        case 1:
            val = 3.0f * x * (1.0f - x) * (1.0f - x); break;
        case 2:
            val = 3.0f * x * x * (1.0f - x); break;
        case 3:
            val = x * x * x; break;
        }
        return val;
    }
    float BernsteinPolyDiff(int i, float x) const
    {
        // derivative of cubic Bernstein polynomial wrt. x
        float val = 1.0f;
        switch (i)
        {
        case 0:
            val = -3.0f * (1.0f - x) * (1.0f - x); break;
        case 1:
            val = 9.0f * x * x - 12.0f * x + 3.0f; break;
        case 2:
            val = 6.0f * x - 9.0 * x * x; break;
        case 3:
            val = 3.0f * x * x; break;
        }
        return val;
    }
    void EvaluateBezierPoint(float u, float v, const std::vector<glm::vec3>& cp, glm::vec3& pos,glm::vec3& norm, glm::vec3& tangent) const
    {
        pos = glm::vec3();
        tangent = glm::vec3();
        glm::vec3 bitangent;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                pos += cp[i * 4 + j] * BernsteinPoly(i, u) * BernsteinPoly(j, v);
                tangent += cp[i * 4 + j] * BernsteinPolyDiff(i, u) * BernsteinPoly(j, v);
                bitangent += cp[i * 4 + j] * BernsteinPoly(i, u) * BernsteinPolyDiff(j, v);
            }
        }
        norm = glm::cross(tangent, bitangent);
        if (glm::dot(norm, norm) < 1e-8)
        {
            norm = glm::vec3(0, 1, 0);
            tangent = glm::vec3(1, 0, 0);
        }
        else
        {
            norm = glm::normalize(norm);
            tangent = glm::normalize(tangent);
        }

    }
    void CreateGeometry(const std::vector<glm::vec3>& controlPoints, int nSubDivisions)
    {
        float delta = 1.0f / (float)nSubDivisions;
        for (int i = 0; i < nSubDivisions; i++)
        {
            for (int j = 0; j < nSubDivisions; j++)
            {
                float u0, u1, v0, v1;
                u0 = i * delta;
                u1 = (i + 1) * delta;
                v0 = j * delta;
                v1 = (j + 1) * delta;
                // evaluate the four corners of a quad here, and add to the vertex array

                glm::vec3 pos[4];
                glm::vec3 norm[4];
                glm::vec3 tang[4];
                glm::vec2 uv[4];

                EvaluateBezierPoint(u0, v0, controlPoints, pos[0], norm[0], tang[0]);
                EvaluateBezierPoint(u0, v1, controlPoints, pos[1], norm[1], tang[1]);
                EvaluateBezierPoint(u1, v1, controlPoints, pos[2], norm[2], tang[2]);
                EvaluateBezierPoint(u1, v0, controlPoints, pos[3], norm[3], tang[3]);

                uv[0] = glm::vec2(u0, v0);
                uv[1] = glm::vec2(u0, v1);
                uv[2] = glm::vec2(u1, v1);
                uv[3] = glm::vec2(u1, v0);

                int indices[6] = { 0,1,2,0,2,3 };
                for (int k = 0; k < 6; k++) {
                    int invert = indices[k];
                    positions.push_back(pos[invert]);
                    normals.push_back(norm[invert]);
                    uvs.push_back(uv[invert]);
                    tangents.push_back(tang[invert]);
                }
            }
        }
    }
public:
    BezierPatch(const std::vector<glm::vec3>& controlPoints, int nSubDivisions) { CreateGeometry(controlPoints, nSubDivisions); }
    const std::vector<glm::vec3>& GetPositionVector() const { return positions; }
    const std::vector<glm::vec3>& GetNormalVector() const { return normals; }
    const std::vector<glm::vec2>& GetUVVector() const { return uvs; }
    const std::vector<glm::vec3>& GetTangentVector() const { return tangents; }
    int GetNumVertices() const { return positions.size(); }
};