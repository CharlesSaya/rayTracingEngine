#include "implicitSurface.hpp"

namespace ISICG_ISIR
{
	/*
	 * Constructeur de classe
	 */


	ImplicitSurface::ImplicitSurface(const Vec3f &center,
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
		_center=center;
	}

	/*
	 * Fonction renvoyant un float en caculée avec le point sur le rayon sur lequel on
	 * marche. La fonction retourne une valeur positive si	l'on est à l'extérieur de la 
	 * surface et une valeur négative si l'on est à l'intérieur.
	 * Une valeur nulle est retourné quand le point appartient à la surface.
	 * La fonction ici représente un cylindre quadrique
	 */

	float ImplicitSurface::distanceImplicitFunction(const Vec3f &position)
	{
		Vec3f pos = (position - _center) * 4.f;
		if (pos.y < 10.f && pos.y > -10.f)
		 return pow(pos.y, 2) * pow(pos.x, 2) + pow(pos.y, 2) * 
				   pow(pos.z, 2) + 0.01 * pow(pos.x,2) + 0.01 * pow(pos.z,2) - 0.01;
		}

	
	/*
	 * Fonction calculant la normale d'une surface implicite à l'aide de sa
	 * représentation implicite
	 */


	Vec3f ImplicitSurface::calcNormal(const Vec3f &pos)
	{
		const Vec2f eps = Vec2f(0.001, 0.0);

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
	 * la surface si elle existe.
	 * La méthode s'arrête lorsque une distance maximale a été atteinte ou si 
	 * la surface a été intersectée par le rayon.
	 */


	std::vector<Intersection> ImplicitSurface::intersect(const Ray &ray)
	{
		std::vector<Intersection> intersections;
		float minDistance = 1;
		float maxDistance = 10;
		float t = 0;
		float step = 0.01;
		float sign=0;
		Vec3f rd = ray.getDirection();
		Vec3f ro = ray.getOrigin();
		Vec3f position = ro;
		float distance = distanceImplicitFunction(position);
		;

		while (t < maxDistance)
		{
			sign = glm::sign(distance);
			distance = distanceImplicitFunction(position);
			if (sign != glm::sign(distance))                          // un changement de signe indicte un passage de l'extérieur à l'intérieur de la surface															
			{														  // ou inversement
				Vec3f normale = calcNormal(position);
				Intersection i(t, position +0.01f*normale , normale, this);
				intersections.push_back(i);
				return intersections;
			}
			t += step;
			position = ro + t * rd;
		}
		return intersections;
	}

} // namespace ISICG_ISIR