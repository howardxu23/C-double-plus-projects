#pragma once
#include "stdafx.h"
class Spline
{
	bool bLoop;
	std::vector<glm::vec3> pControl;
	// basis functions
protected:
	virtual float b0(float u) const = 0;
	virtual float b1(float u) const = 0;
	virtual float b2(float u) const = 0;
	virtual float b3(float u) const = 0;

public:
	Spline(const std::vector<glm::vec3>& controlPoints, int bLoop = false) : pControl(controlPoints), bLoop(bLoop) {}
	int GetNumPoints() const { return pControl.size(); }
	glm::vec3 GetPoint(float u) const
	{
		int n = pControl.size();
		if (n < 4)
		{
			std::cerr << "not enough control points for a Spline" << std::endl;
			return glm::vec3(0, 0, 0);
		}
		// find which segment are we in, and which control points to use
		int iu = (int)u;
		u -= iu;
		int nmax = bLoop ? n : n - 2;
		if (iu < 0)
		{
			// clamp
			iu = 0; u = 0.0f;
		}
		if (iu > nmax)
		{
			// clamp
			iu = nmax; u = 0.999999f;
		}
		// get the control points
		glm::vec3 p[4];

		if (bLoop)
		{
			p[0] = pControl[(iu - 1 + n) % n];
			p[1] = pControl[iu % n];
			p[2] = pControl[(iu + 1) % n];
			p[3] = pControl[(iu + 2) % n];
		}
		else
		{
			// p[1] and p[2] should be fine
			p[1] = pControl[iu];
			p[2] = pControl[iu + 1];
			// p[0] and p[3] need special treatment at the ends.
			if (iu == 0)
				p[0] = pControl[0] + (pControl[0] - pControl[1]);
			else
				p[0] = pControl[iu - 1];
			if (iu == n - 2)
				p[3] = pControl[n - 1] + (pControl[n - 1] - pControl[n - 2]);
			else
				p[3] = pControl[iu + 2];
		}

		// now work out the point
		glm::vec3 r = p[0] * b0(u) + p[1] * b1(u) + p[2] * b2(u) + p[3] * b3(u);
		return r;
	}
};
class  BSpline : public Spline
{
	virtual float b0(float u) const 
	{
		return(1 - u) * (1 - u) * (1 - u) * (1.0f / 6.0f);
	}
	virtual float b1(float u) const 
	{
		return(3 * u * u * u - 6 * u * u + 4) * (1.0f / 6.0f);
	}
	virtual float b2(float u) const 
	{
		return(-3 * u * u + 3 * u * u + 3 * u + 1) * (1.0f / 6.0f);
	}
	virtual float b3(float u) const 
	{
		return u * u * u * (1.0f / 6.0f);
	}
public:
	BSpline(const std::vector<glm::vec3>& points, bool loop = false) :
		Spline(points, loop) {}

};
class  CRspline : public Spline
{
	float t;
	virtual float b0(float u) const
	{
		return -t * u + 2 * t * u * u - t * u * u * u;
	}
	virtual float b1(float u) const
	{
		return 1 + (t - 3) * u * u + (2 - t) * u * u * u;
	}
	virtual float b2(float u) const
	{
		return t* u + (3 - 2 * t) * u * u + (t - 2) * u * u * u;
	}
	virtual float b3(float u) const
	{
		return -t * u * u + t * u * u * u;
	}
public:
	CRspline(const std::vector<glm::vec3>& points, bool loop = false, float tension = 0.5f ) :
		Spline(points, loop), t(tension) {}

};