#ifndef __ISICG_ISIR_RAY__
#define __ISICG_ISIR_RAY__

#include "defines.hpp"

namespace ISICG_ISIR
{

	/*
	 * Classe contenant les données d'un rayon
	 */


	class Ray
	{
	public:

		Ray() {}

		Ray(const Vec3f &origin, const Vec3f &direction)
			: _origin(origin),_direction(direction)
		{
		}

		Vec3f getOrigin() const ;
		
		Vec3f getDirection() const; 


	private:
		Vec3f _origin;
		Vec3f _direction;

	};
} // namespace ISICG_ISIR

#endif // __ISICG_ISIR_RAY__
