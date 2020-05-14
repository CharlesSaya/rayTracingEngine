#ifndef __ISICG_ISIR_A_OBJECT_3D__
#define __ISICG_ISIR_A_OBJECT_3D__

#include "3D/intersection.hpp"
#include "defines.hpp"
#include "materials/aMaterial.hpp"
#include <vector>

namespace ISICG_ISIR
{

	/*
	 * Classe mère représentant la famille de nos objets 3D de la scène
	 */

	class AObject3D
	{
	public:

		void setMaterial(AMaterial *const &material);

		void setColorMaterial(AMaterial *const &colorMaterial);

		AMaterial * getColorMaterial();

		AMaterial *getMaterial();
		
		const float getReflectionAmount();

		const float getRefractionAmount();

		const float getRefractionIndex();

		const float getRugosity();

		const Vec3f& getf0();

		void setReflectionAmount(float refl);
		
		void setRefractionAmount(float refr);
		
		void setRefractionIndex(float refrI);

		void setRugosity(float rugosity);

		void setf0(const Vec3f &f0);

				
		// Compute ray/object intersection
		// return parameter t and set normal if intersection exists
		virtual std::vector<Intersection> intersect(const Ray &ray) = 0;


	protected:
		AMaterial *_material = nullptr;  // pointer for polymorphism

		AMaterial * _colorMaterial = nullptr; //pointer for polymorphism

		float _reflectionAmount=0.f;     //Montant de réflection de l'objet

		float _refractionAmount = 0.f; // Montant de refraction de l'objet

		float _refractionIndex = 1.f; // Indice de refraction de l'objet

		float _rugosity = 0.5f; // Montant de réflection de l'objet

		Vec3f _f0;  // Coefficient de Fresnel de l'objet
	};	
} // namespace ISICG_ISIR

#endif // __ISICG_ISIR_A_OBJECT_3D__
