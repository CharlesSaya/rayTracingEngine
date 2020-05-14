#include "intersection.hpp"

namespace ISICG_ISIR
{
	
	/*
	 * Constructeur de structure
	 */


	Intersection::Intersection(float distance, const Vec3f &position, const Vec3f &normale,
				 AObject3D *intersectingObject)
	{
		this->_distance = distance;
		this->_position = position;
		this->_normale = normale;
		this->_object = intersectingObject;
	}

}