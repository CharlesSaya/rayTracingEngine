#ifndef __ISICG_ISIR_BOX__
#define __ISICG_ISIR_BOX__

#include "defines.hpp"
#include <vector>
#include "3D/intersection.hpp"
#include "3D/ray.hpp"
#include "3D/triangleMesh.hpp"

namespace ISICG_ISIR
{

	/*
	 * Classe représentant une boite englobant les triangles d'un mesh.
	 * Elle est utilisée lors du calcul de BVH.
	 */


	class Box 
	{
		public:

			Box();

			Box(const Vec3f &bMin, const Vec3f &bMax) : _minBounds(bMin), _maxBounds(bMax)
			{	
			}

			bool isEmpty() const;

			void emptyBox();

			void enlarge(const Vec3f &pt);

			const float getArea();

			bool intersect(const Ray &ray);


		private:	
			Vec3f _minBounds,_maxBounds;						//Coordonnées des frontières de la boite
	};

} // namespace ISICG_ISIR

#endif //__ISICG_ISIR_BOX__
