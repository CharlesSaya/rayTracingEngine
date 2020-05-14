#ifndef __ISICG_ISIR_LIGHT_MATERIAL__
#define __ISICG_ISIR_LIGHT_MATERIAL__
#define _USE_MATH_DEFINES

#include "3D/aObject3D.hpp"
#include "defines.hpp"
#include "materials/aMaterial.hpp"
#include <vector>
#include "BVH/bvhnode.hpp"
#include "brdf.hpp"
#include "light/light.hpp"
#include "camera.hpp"
#include <math.h>

namespace ISICG_ISIR
{
	/*
	 * Classe implémentantles fonctions  pour récupérer la 
	 * couleur de l'objet en un point à partir des données 
	 * de son matériau
	 */

	class LightMaterial : public AMaterial
	{
	public:
		LightMaterial(const std::vector<Light *> &lights, const Camera &_camera,
					  int limit);

		Vec3f shade(const Ray &ray, const Intersection &hitPoint);

		Vec3f getColor(const Ray &ray, const Intersection &currentIntersection,
					int loop);

		int calculateShadow(const Intersection &hitpoint, Light *&light,
							  Vec3f &color, AObject3D *&currentObject);

		void setObjects(std::vector<AObject3D *> &objects);

		void setBVH(std::vector<BVHNode *> &bvh);

	private:
		int _limit;									//Limite de récursion de la fonction getColor
		BRDF _brdf;									//BRDF  
		std::vector<Light *> _lights;				//Lumières de la scène
		Camera _camera;								//Caméra 
		std::vector<AObject3D *> _objects;			//Objets de la scène
		std::vector<BVHNode *> _bvh;				//Objets BVH de la scène
	};

}; // namespace ISICG_ISIR
#endif