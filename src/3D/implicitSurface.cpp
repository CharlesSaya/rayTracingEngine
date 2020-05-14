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
	 * Fonction renvoyant un float en cacul�e avec le point sur le rayon sur lequel on
	 * marche. La fonction retourne une valeur positive si	l'on est � l'ext�rieur de la 
	 * surface et une valeur n�gative si l'on est � l'int�rieur.
	 * Une valeur nulle est retourn� quand le point appartient � la surface.
	 * La fonction ici repr�sente un cylindre quadrique
	 */

	float ImplicitSurface::distanceImplicitFunction(const Vec3f &position)
	{
		Vec3f pos = (position - _center) * 4.f;
		if (pos.y < 10.f && pos.y > -10.f)
		 return pow(pos.y, 2) * pow(pos.x, 2) + pow(pos.y, 2) * 
				   pow(pos.z, 2) + 0.01 * pow(pos.x,2) + 0.01 * pow(pos.z,2) - 0.01;
		}

	
	/*
	 * Fonction calculant la normale d'une surface implicite � l'aide de sa
	 * repr�sentation implicite
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
	 * une m�thode de ray-marching pour trouver l'intersection entre le rayon et
	 * la surface si elle existe.
	 * La m�thode s'arr�te lorsque une distance maximale a �t� atteinte ou si 
	 * la surface a �t� intersect�e par le rayon.
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
			if (sign != glm::sign(distance))                          // un changement de signe indicte un passage de l'ext�rieur � l'int�rieur de la surface															
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