#include "implicitSphere.hpp"

namespace ISICG_ISIR
{

	/*
	 * Constructeur de classe
	 */


	ImplicitSphere::ImplicitSphere(const Vec3f &center, const float radius,
								   const float reflectionAmount,
								   const float refractionAmount,
								   const float refractionIdx,
								   const float rugosity, const Vec3f &f0)
	{
		_reflectionAmount = reflectionAmount;
		_refractionAmount = refractionAmount;
		_refractionIndex = refractionIdx;
		_rugosity = rugosity;
		_f0 = f0;
		_radius = radius;
		_center = center;
	}

	/*
	 * Fonction renvoyant la distance entre un point sur le rayon sur lequel on marche 
	 * et la surface de la sphère
	 */

	float ImplicitSphere::distanceImplicitFunction(const Vec3f &position)
	{
		return glm::length((position - _center)) - _radius;
	}

	/*
	 * Fonction calculant la normale d'une surface implicite à l'aide de sa représentation implicite
	 */

	Vec3f ImplicitSphere::calcNormal(const Vec3f &pos)
	{
		const Vec2f eps = Vec2f(0.0001, 0.0);

		Vec3f nor = Vec3f(
			distanceImplicitFunction(pos + Vec3f(eps.x, eps.y, eps.y))
				- distanceImplicitFunction(pos - Vec3f(eps.x, eps.y, eps.y)),

			distanceImplicitFunction(pos + Vec3f(eps.y, eps.x, eps.y))
				- distanceImplicitFunction(pos - Vec3f(eps.y, eps.x, eps.y)),

			distanceImplicitFunction(pos + Vec3f(eps.y, eps.y, eps.x))
				- distanceImplicitFunction(pos - Vec3f(eps.y, eps.y, eps.x))
				
				);

		return normalize(nor);
	}

	/*
	 * Fonction d'intersection. Dans le cas des surfaces implicites, on effectue
	 * une méthode de ray-marching pour trouver l'intersection entre le rayon et la surface
	 * si elle existe
	 */

	std::vector<Intersection> ImplicitSphere::intersect(const Ray &ray)
	{
		std::vector<Intersection> intersections;
		float minDistance = 1;
		float maxDistance = 15;
		float t = 0;
		float distance = 0;
		Vec3f rd = ray.getDirection();
		Vec3f ro = ray.getOrigin();
		Vec3f position = ro;
		while (t < maxDistance)
		{
			distance = distanceImplicitFunction(position);
			if (distance <	 10e-7 )
			{
				Vec3f normale
					= calcNormal(position);
				Intersection i(t, position + normale *0.00001f,normale , this);
				intersections.push_back(i);
				return intersections;
			}
			t += distance;
			position = ro + t * rd;
		}
		return intersections;
	}

} // namespace ISICG_ISIR