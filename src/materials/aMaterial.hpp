#ifndef __ISICG_ISIR_A_MATERIAL__
#define __ISICG_ISIR_A_MATERIAL__

#include "3D/intersection.hpp"
#include "3D/ray.hpp"

namespace ISICG_ISIR
{
	/*
	 * Classe m�re repr�sentant les mat�riaux
	 */

	class AMaterial
	{
	public:

		//Fonction permettant de r�cup�rer la couleur de l'objet intersect�
		//� la position pos
		virtual Vec3f shade(const Ray &ray, const Intersection &pos) = 0;

	protected:



	};
} // namespace ISICG_ISIR

#endif // __ISICG_ISIR_A_MATERIAL__
