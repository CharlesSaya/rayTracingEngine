#ifndef __ISICG_ISIR_IMPLICIT_SURFACE
#define __ISICG_ISIR_IMPLICIT_SURFACE

#	include "aObject3D.hpp"
#	include "defines.hpp"
#	include <vector>

namespace ISICG_ISIR
{

	/*
	 * Classe représentant une surface 3D calculée de manière implicite
	 */


	class ImplicitSurface : public AObject3D
	{
	public:
		ImplicitSurface() = default;

		ImplicitSurface(const Vec3f &center, const float reflectionAmount,
						const float refractionAmount, const float refractionIdx,
						const float rugosity, const Vec3f &f0);

		float distanceImplicitFunction(const Vec3f &position);

		Vec3f calcNormal(const Vec3f &pos);

		std::vector<Intersection> intersect(const Ray &ray);

	private:
		Vec3f _center;
	};
} // namespace ISICG_ISIR
#endif
