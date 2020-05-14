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
	 * Fonction permettant de récuperer l'intensité de la lumière en fonction de
	 * la distance entre un point et le centre de la lumière
	 */

	float PointLight::getIntensity(const float length)
	{
		float intensity	= (_intensity) /M_PI * 4.f* (length * length);

		intensity = glm::clamp(intensity,0.f,1.0f);

		return intensity;

	}
			
}

