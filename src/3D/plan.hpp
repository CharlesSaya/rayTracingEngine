#ifndef __ISICG_ISIR_PLANE__
#define __ISICG_ISIR_PLANE__

#include "aObject3D.hpp"
#include "defines.hpp"
#include <3D/triangleMesh.hpp>
#include <vector>

namespace ISICG_ISIR
{
	
	/*
	 * Classe  représentant un plan 3D. 
	 * Hérite de AObject3D
	 * Le plan est réprésenté à l'aide de sa normale et d'un point appartenant
	 * à celui ci.
	 */


	class Plan : public AObject3D
	{
	public:
		Plan() = default;

		Plan(const Vec3f &normale, const Vec3f &p1, float reflectionAmount,
			 float refractionAmount, float refractionIdx, float rugosity,
			 Vec3f f0);

		std::vector<Intersection> intersect(const Ray &ray);

	private:
		Vec3f _p1;				 //Point appartenant au plan
		Vec3f _normale;          //Normale du plan
	};
} // namespace ISICG_ISIR

#endif // __ISICG_ISIR_PLANE__
