#include "RayCastSphere.h"

#include <math.h> 

// fonction d'intersection entre la sph�re et un rayon
double RayCastSphere::intersect(const RayCastRay& ray) const
{
	// distance de l'intersection la plus pr�s si elle existe
	double distance;

	// seuil de tol�rance num�rique du test d'intersection
	double epsilon = 1e-4;

	// distance du pointsize d'intersection
	double t;

	// vecteur entre le centre de la sph�re et l'origine du rayon
	Vector delta = position - ray.origin;

	// calculer a
	double a = delta.dot(delta);

	// calculer b
	double b = delta.dot(ray.direction);

	// calculer c
	double c = radius * radius;

	// calculer le discriminant de l'�quation quadratique
	double discriminant = b * b - a + c;

	// valider si le discriminant est n�gatif
	if (discriminant < 0)
	{
		// il n'y a pas d'intersection avec cette sph�re
		return distance = 0;
	}

	// calculer la racine carr�e du discriminant seulement si non-n�gatif
	discriminant = sqrt(discriminant);

	// d�terminer la distance de la premi�re intersection
	t = b - discriminant;

	// valider si la distance de la premi�re intersection est dans le seuil de tol�rance
	if (t > epsilon)
		distance = t;
	else
	{
		// d�terminer la distance de la premi�re intersection
		t = b + discriminant;

		// valider si la distance de la seconde intersection est dans le seuil de tol�rance
		if (t > epsilon)
			distance = t;
		else
			distance = 0;
	}

	// retourner la distance du point d'intersection
	return distance;
}