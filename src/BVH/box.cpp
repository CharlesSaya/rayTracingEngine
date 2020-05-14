#include "box.hpp"

namespace ISICG_ISIR
{
	
	/*
	 * Constructeur de classe
	 */

	Box::Box() {
		_minBounds = Vec3f(FLT_MAX);
		_maxBounds = Vec3f(-FLT_MAX);
	}

	/*
	 * Fonction permettant de savoir si la boite contient des triangles
	 */

	bool Box::isEmpty() const
	{

		return _minBounds == Vec3f(FLT_MAX) && _maxBounds == Vec3f(-FLT_MAX);

	}

	/*
	 * Fonction réinitialisant les frontières de la boite
	 */

	void Box::emptyBox() {
		_minBounds = Vec3f(FLT_MAX);
		_maxBounds = Vec3f(-FLT_MAX);
	}

	/*
	 * Fonction élargissant la boite si nécessaire, en fonction
	 * des coordonnées du point passé en paramètre
	 */

	void Box::enlarge(const Vec3f &pt)
	{
		
		_minBounds.x = glm::min(pt.x, _minBounds.x);
		_minBounds.y = glm::min(pt.y, _minBounds.y);
		_minBounds.z = glm::min(pt.z, _minBounds.z);
	
		_maxBounds.x = glm::max(pt.x, _maxBounds.x);
		_maxBounds.y = glm::max(pt.y, _maxBounds.y);
		_maxBounds.z = glm::max(pt.z, _maxBounds.z);

	}

	/*
	 * Fonction permettant de calculer le périmètre d'une boite
	 */
	
	const float Box::getArea()
	{
		return 4
			   * ((_maxBounds.x - _minBounds.x) + (_maxBounds.y - _minBounds.y)
				  + (_maxBounds.z - _minBounds.z));
	}

	/*
	 * Fonction d'intersection. Retourne un booléen en fonction 
	 * de si il en existe une ou pas.
	 */

	bool Box::intersect(const Ray& ray) {

		Vec3f rayDirection = ray.getDirection();
		Vec3f rayOrigin = ray.getOrigin();

		float t0x = (_minBounds.x - rayOrigin.x) / rayDirection.x;
		float t1x = (_maxBounds.x - rayOrigin.x) / rayDirection.x;


		if (t0x > t1x)
			std::swap(t0x, t1x);

		float tMin = t0x;
		float tMax = t1x;

		float t0y = (_minBounds.y - rayOrigin.y) / rayDirection.y;
		float t1y = (_maxBounds.y - rayOrigin.y) / rayDirection.y;
	
		if (t0y > t1y)
			std::swap(t0y, t1y);

		if (tMin > t1y || tMax > t1x)
			return false;

		tMin = (t0x > t0y) ? t0x : t0y;
		tMax = (t1x > t1y) ? t1y : t1x;

		float t0z = (_minBounds.z - rayOrigin.z) / rayDirection.z;
		float t1z = (_maxBounds.z - rayOrigin.z) / rayDirection.z;

		if (t0z > t1z)
			std::swap(t0z, t1z);

		if (tMin > t1z || t0z > tMax)
			return false;

		if (t0z > tMin)
			tMin = t0z; 
			
		if (t1z < tMax)
			tMax = t1z;

		return true;
	}



}//namespace ISICG_ISIR