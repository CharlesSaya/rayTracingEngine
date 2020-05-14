#ifndef __ISICG_ISIR_SPHERE__
#define __ISICG_ISIR_SPHERE__

#include <vector>

#include "aObject3D.hpp"

namespace ISICG_ISIR
{
	
	/*
	 * Classe représentant une sphère 3D.
	 * Hérite de AObject3D
	 */


	class Sphere : public AObject3D
	{
	public:
		Sphere() = default;

		Sphere(const Vec3f &center, const float radius,
			   const float reflectionAmount, const float refractionAmount,
			   const float refractionIndex, const float rugosity,
			   const Vec3f &f0);

		std::vector<Intersection> intersect(const Ray &ray);

	private:
		Vec3f _center = VEC3F_ZERO;
		float _radius = 1.f;
	};
} // namespace ISICG_ISIR

#endif //__ISICG_ISIR_SPHERE__
