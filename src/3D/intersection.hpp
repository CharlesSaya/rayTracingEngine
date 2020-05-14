#ifndef __ISICG_ISIR_INTERSECTION__
#define __ISICG_ISIR_INTERSECTION__

#include "defines.hpp"
#include <limits>

namespace ISICG_ISIR
{
	class AObject3D;

	/*
	 * Structure contenant les données d'intersection entre un rayon et un objet
	 */


	struct Intersection
	{
		Intersection() = default;

		Intersection(float distance, const Vec3f &position,
					 const Vec3f &normale, AObject3D *intersectingObject);

		AObject3D * _object = nullptr;
		Vec3f _normale = VEC3F_ZERO;	
		Vec3f _position = VEC3F_ZERO;
		float _distance = -1.f;
	};
} // namespace ISICG_ISIR

#endif //__ISICG_ISIR_INTERSECTION__
