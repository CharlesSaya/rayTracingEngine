#ifndef __ISICG_ISIR_LIGHT__
#define __ISICG_ISIR_LIGHT__

#include "defines.hpp"
namespace ISICG_ISIR
{
	/*
	 * Classe mère représentant les lumières
	 */

	class Light
	{
	public:
		Light() = default;
		Light(const Vec3f &color, const Vec3f &position);

		Vec3f& getPosition();
		Vec3f& getColor();

		//Fonction virtuelle implémenté par les classes héritant de celle ci
		virtual float getIntensity(const float length);
		
		
		protected: 
			Vec3f _color;
			Vec3f _position;

	};
}

#endif