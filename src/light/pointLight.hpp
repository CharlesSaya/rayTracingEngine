#ifndef __ISICG_ISIR_POINT_LIGHT__
#define __ISICG_ISIR_POINT_LIGHT__

#include "defines.hpp"
#include "light.hpp"

namespace ISICG_ISIR
{

	/*
	 * Classe représentant les lumières sphériques
	 * Hérite de Light
	 */
	
	class PointLight : public Light
	{
	public:
		
		PointLight() = default;
		PointLight(const Vec3f &color, const Vec3f &position, const float intensity);
		float getIntensity(const float length) override;

	private:
		float _intensity;
	};

}
#endif