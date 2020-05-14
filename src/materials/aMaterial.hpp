#ifndef __ISICG_ISIR_A_MATERIAL__
#define __ISICG_ISIR_A_MATERIAL__

#include "3D/intersection.hpp"
#include "3D/ray.hpp"

namespace ISICG_ISIR
{
	/*
	 * Classe mère représentant les matériaux
	 */

	class AMaterial
	{
	public:

		//Fonction permettant de récupérer la couleur de l'objet intersecté
		//à la position pos
		virtual Vec3f shade(const Ray &ray, const Intersection &pos) = 0;

	protected:



	};
} // namespace ISICG_ISIR

#endif // __ISICG_ISIR_A_MATERIAL__
