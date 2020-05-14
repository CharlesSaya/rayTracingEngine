#include "implicitCylinder.hpp"

namespace ISICG_ISIR
{
	/*
	 * Constructeur de classe
	 */

	ImplicitCylinder::ImplicitCylinder(const Vec3f &center, const float radius,
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
	 * Fonction renvoyant un float en caculée avec le point sur le rayon sur
	 * lequel on marche. La fonction retourne une valeur positive si	l'on est
	 * à l'extérieur de la surface et une valeur négative si l'on est à
	 * l'intérieur. Une valeur nulle est retourné quand le point appartient à la
	 * surface.
	 */

	float ImplicitCylinder::distanceImplicitFunction(const Vec3f &position)
	{
		Vec3f pos = (position - _center);
		if (pos.y<10.f && pos.y>-2.f)
			return pow(pos.x, 2) + pow(pos.z, 2) - (_radius * _radius);
	}

	/*
	 * Fonction calculant la normale d'une surface implicite à l'aide de sa
	 * représentation implicite
	 */

	Vec3f ImplicitCylinder::calcNormal(const Vec3f &pos)
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
	 * une méthode de ray-marching pour trouver l'intersection entre le rayon et
	 * la surface si elle existe
	 */

	std::vector<Intersection> ImplicitCylinder::intersect(const Ray &ray)
	{
		std::vector<Intersection> intersections;
		float minDistance = 1;
		float maxDistance = 10;
		float t = 0;
		float step = 0.01;
		float epsilon = 0.000001;
		float sign = 0;
		Vec3f rd = ray.getDirection();
		Vec3f ro = ray.getOrigin();
		Vec3f position = ro;
		float distance = distanceImplicitFunction(position);

		while (t < maxDistance)
		{
			sign = glm::sign(distance);
			distance = distanceImplicitFunction(position);
			if (sign!= glm::sign(distance))						// un changement de signe indicte un passage de
																// l'extérieur à l'intérieur de la surface ou inversement
			{													
				Vec3f normale = calcNormal(position);
				Intersection i(t, position +normale * 0.01f, normale,
							   this);
				intersections.push_back(i);
				return intersections;
			}
			t += step;
			position = ro + t * rd;
		}
		return intersections;
	}
} // namespace ISICG_ISIR
