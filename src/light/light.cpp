#include "light.hpp"

namespace ISICG_ISIR
{

	/*
	 * Constructeur de classe
	 */

	Light::Light(const Vec3f &color, const Vec3f &position)
		: _color(color), _position(position)
	{
	}

	/*  
	 * Fonction permettant de récuperer l'intensité de la lumière
	 */

	float Light::getIntensity(const float length)
	{
		return (1.0f);
	}

	/* 
	 * Getters des variables de la classe
	 */

	
	Vec3f &Light::getColor()
	{
		return _color;
	}

	Vec3f &Light::getPosition()
	{
		return _position;
	}


}