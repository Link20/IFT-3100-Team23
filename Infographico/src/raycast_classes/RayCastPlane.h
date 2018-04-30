#pragma once

#include "Vector.h"
#include "RayCastRay.h"
#include "SurfaceType.h"

class RayCastPlane
{

public:
	
	double radius;   // rayon de la sph�re
	Vector position; // position du centre de la sph�re
	Vector emission; // couleur �missive de la sph�re
	Vector color;    // couleur diffuse de la sph�re

	SurfaceType material; // type de r�flexion de la sph�re

							// constructeur
	RayCastPlane(double r, Vector p, Vector e, Vector c, SurfaceType m) : radius(r), position(p), emission(e), color(c), material(m) {}
	double intersect(const RayCastRay& ray) const;
	
};

