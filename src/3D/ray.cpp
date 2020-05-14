#include "ray.hpp"

namespace ISICG_ISIR
{

	/*
	 * Getters des param�tres de la classe
	 */

	Vec3f Ray::getOrigin() const
	{
		return _origin;
	}

	Vec3f Ray::getDirection() const
	{
		return _direction;
	}

}