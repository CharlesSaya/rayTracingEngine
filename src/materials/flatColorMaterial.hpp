#ifndef __ISICG_ISIR_FLAT_COLOR_MATERIAL__
#define __ISICG_ISIR_FLAT_COLOR_MATERIAL__

#include "materials/aMaterial.hpp"

namespace ISICG_ISIR
{
	
	/*
	 * Materiel contant l'albedo de l'objet
	 */

	
	class FlatColorMaterial : public AMaterial
	{

	public:

		
	
		FlatColorMaterial(const Vec3f &color) : _color(color) {}


		Vec3f shade(const Ray &ray, const Intersection &pos)
		{
			return _color;
		}
	private:
		Vec3f _color = VEC3F_ZERO;
	};
} // namespace ISICG_ISIR

#endif // __ISICG_ISIR_FLAT_COLOR_MATERIAL__
