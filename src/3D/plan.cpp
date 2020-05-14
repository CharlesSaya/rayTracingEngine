#include "plan.hpp"
#include <vector>

namespace ISICG_ISIR
{
	/*
	 * Constructeur de classe
	 */

	Plan::Plan(const Vec3f &normale, const Vec3f &p1, float reflectionAmount,
			   float refractionAmount, float refractionIdx, float rugosity,
			   Vec3f f0)
		: _normale(normale), _p1(p1)
	{
		_reflectionAmount = reflectionAmount;
		_refractionAmount = refractionAmount;
		_refractionIndex = refractionIdx;
		_rugosity = rugosity;
		_f0 = f0;
	}

	/*
	 * Fonction d'intersection avec un plan. On utilise la représentation 
	 * paramétrique du plan pour pouvoir calculer l'intersection entre le rayon et
	 * le plan si elle existe.
	 */


	std::vector<Intersection> Plan::intersect(const Ray &ray)
	{
		std::vector<Intersection> intersection;
		float d = -glm::dot(_normale, _p1);
		float A = glm::dot(_normale, ray.getDirection());
		float B = glm::dot(_normale, ray.getOrigin()) + d;
		float t = -B / A;
		if (t > 0)
		{
			Vec3f p = ray.getOrigin() + ray.getDirection() * t;
			Intersection inter(t, p + _normale *0.0001f, _normale, this);

			if (inter._distance >= 10e-4)
				intersection.push_back(inter);
		}
		return intersection;
	}

} // namespace ISICG_ISIR