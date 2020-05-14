#ifndef __ISICG_ISIR_CHECKERED_MATERIAL__
#define __ISICG_ISIR_CHECKERED_MATERIAL__

#include "defines.hpp"
#include "aMaterial.hpp"

namespace ISICG_ISIR
{
	
	/*
	 * Matériel réprésentant un damier
	 * Hérite de aMaterial
	 */


	class CheckeredMaterial : public AMaterial
	{
		public:

			CheckeredMaterial();

			Vec3f shade(const Ray &ray, const Intersection &pos);

		private:
	};
} // namespace ISICG_ISIR
#endif