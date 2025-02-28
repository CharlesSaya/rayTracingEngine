#define _USE_MATH_DEFINES
#include "pointLight.hpp"
#include <math.h>

namespace ISICG_ISIR
{
	/*
	 * Constructeur de classe
	 */


	PointLight::PointLight(const Vec3f &color, const Vec3f &position, const		float intensity)
	{
		_color = color;
		_position = position;
		_intensity = intensity;
	}

	/*
	 * Fonction permettant de r�cuperer l'intensit� de la lumi�re en fonction de
	 * la distance entre un point et le centre de la lumi�re
	 */

	float PointLight::getIntensity(const float length)
	{
		float intensity	= (_intensity) /M_PI * 4.f* (length * length);

		intensity = glm::clamp(intensity,0.f,1.0f);

		return intensity;

	}
			
}

