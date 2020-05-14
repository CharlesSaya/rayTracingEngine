#ifndef __ISICG_ISIR_CAMERA__
#define __ISICG_ISIR_CAMERA__

#include "3D/ray.hpp"
#include "defines.hpp"

namespace ISICG_ISIR
{
	
	/*
	 * Classe représentant la caméra de la scène
	 */

	class Camera
	{
	public:
		Camera();
		Camera(uint width, uint height, float focale);

		Ray generateRay(Vec2f pixelPos);
		Vec3f getPosition();

	private:
		Vec3f _posCam = VEC3F_ZERO;
		Vec3f _direction = Vec3f(0.0f, 0.0f, 1.0f);
		float _ratio;
		float _focale;
		Vec3f _up = Vec3f(0.0f, 1.0f, 0.0f);

	};
} // namespace ISICG_ISIR

#endif //__ISICG_ISIR_CAMERA__
