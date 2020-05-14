#include "aObject3D.hpp"
namespace ISICG_ISIR
{

	/*
	* Les fonctions suivantes sont simplement des getters et setters 
	* des paramètres de l'objet
	*/
	
	void AObject3D::setMaterial(AMaterial *const &material)
	{
		_material = material;
	}

	void AObject3D::setColorMaterial(AMaterial *const &colorMaterial)
	{
		_colorMaterial = colorMaterial;
	}

	AMaterial* AObject3D::getMaterial() 
	{
		return _material;
	}

	AMaterial *AObject3D::getColorMaterial()
	{
		return _colorMaterial;
	}

	const float AObject3D::getReflectionAmount()
	{
		return _reflectionAmount;
	}

	const float AObject3D::getRefractionAmount()
	{
		return _refractionAmount;
	}

	const float AObject3D::getRefractionIndex()
	{
		return _refractionIndex;
	}

	const float AObject3D::getRugosity()
	{
		return _rugosity;
	}

	const Vec3f& AObject3D::getf0()
	{
		return _f0;
	}


	void AObject3D::setReflectionAmount(float refl)
	{
		_reflectionAmount = refl;
	}

	void AObject3D::setRefractionAmount(float refr)
	{
		_refractionAmount = refr;
	}

	void AObject3D::setRefractionIndex(float refrI)
	{
		_refractionIndex = refrI;
	}

	void AObject3D::setRugosity(float rugosity) {
		_rugosity = rugosity;
	}

	void AObject3D::setf0(const Vec3f& f0)
	{
		_f0 = f0;
	}

} // namespace ISICG_ISIR