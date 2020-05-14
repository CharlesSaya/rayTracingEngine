

#include "checkeredMaterial.hpp"
#include "materials/aMaterial.hpp"

namespace ISICG_ISIR
{
	
	/*
	 * Constructeur de classe
	 */

	CheckeredMaterial::CheckeredMaterial() {

	}

	/*
	 * Fonction permettant de récupérer la couleur en un point donné
	 */

	Vec3f CheckeredMaterial::shade(const Ray &ray, const Intersection &pos)
	{
		float x, y, z;
		x = floor(pos._position.x);
		y = floor(pos._position.y);
		z = floor(pos._position.z);
		if (fmod((x + y + z), 2) != 0.f)
			return Vec3f(1.f);
		else
			return Vec3f(0.0f,0.0f,1.0f);
	}

} // namespace ISICG_ISIR

