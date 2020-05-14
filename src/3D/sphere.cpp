#include "sphere.hpp"

namespace ISICG_ISIR
{

	/*
	 * Constructeur de classe
	 */


	Sphere::Sphere(const Vec3f &center, const float radius,
				   const float reflectionAmount, const float refractionAmount,
				   const float refractionIndex, const float rugosity,
				   const Vec3f &f0)
		: _center(center), _radius(radius)
	{
		_reflectionAmount = reflectionAmount;
		_refractionIndex = refractionIndex;
		_refractionAmount = refractionAmount;
		_rugosity = rugosity;
		_f0 = f0;
	}

	/*
	 * Fonction d'intersection. Dans le cas de la sphère , on utilise la représentation
	 * paramétrique de celle-ci pour trouver l'intersection entre le rayon et
	 * la sphère
	 */


	std::vector<Intersection> Sphere::intersect(const Ray &ray)
	{
		std::vector<Intersection> intersections;
		float a = dot(ray.getDirection(), ray.getDirection());

		float b = 2 * dot((ray.getOrigin() - _center), ray.getDirection());

		float c = dot((ray.getOrigin() - _center), (ray.getOrigin() - _center))
				  - _radius * _radius;

		float delta = b * b - (4 * a * c);
		if (delta >= 0)
		{
			float t = (-b - sqrt(delta)) / 2 * a;
			if (t > 0)									//l'origine du rayon est devant la sphère et l'intersecte
			{
				float t2 = (-b + sqrt(delta)) / 2 * a;
				Vec3f p = ray.getOrigin() + ray.getDirection() * t;
				Vec3f p2 = ray.getOrigin() + ray.getDirection() * t2;
				Vec3f normale1 = glm::normalize(p - _center);
				Intersection intersect1(t, p+ normale1*0.0001f , normale1,this);

				if (intersect1._distance >= 10e-4)

					intersections.push_back(intersect1);
				Vec3f normale2= glm::normalize(p2 - _center);

				Intersection intersect2(t2, p2 + normale2 * 0.0001f,normale2,
										this);

				if (intersect2._distance >= 10e-4)
					intersections.push_back(intersect2);
			}
			else										 // l'origine du rayon est à l'intérieur la sphère et l'intersecte
			{
				float t2 = (-b + sqrt(delta)) / 2 * a;
				Vec3f p2 = ray.getOrigin() + ray.getDirection() * t2;
				Vec3f normale2 = glm::normalize(p2 - _center);

				Intersection intersect1(t2, p2 + normale2 * 0.001f, normale2,
										this);

				if (intersect1._distance >= 10e-4)
					intersections.push_back(intersect1);
			}
		}

		return intersections;
	}
} // namespace ISICG_ISIR