#include "lightMaterial.hpp"

namespace ISICG_ISIR
{
	/*
	 * Constructeur de classe
	 */

	LightMaterial::LightMaterial(const std::vector<Light *> &lights,
								 const Camera &camera, int limit)
		: _camera(camera), _lights(lights), _limit(limit)
	{
	}

	/*
	 * Ici on va vérifier si le point est dans l'ombre
	 * et modifire son intensité lumineuse si besoin
	 */

	int LightMaterial::calculateShadow(const Intersection &hitpoint,
										 Light *&light, Vec3f &color,
										 AObject3D *&currentObject)
	{
		
		Vec3f lightVector = light->getPosition() - hitpoint._position;
		Vec3f normalizedLightVector = glm::normalize(lightVector);

		Ray shadowRay(hitpoint._position, normalizedLightVector);

		std::vector<std::vector<Intersection>> shadowIntersections;

		for (AObject3D *o : _objects)
		{
			shadowIntersections.push_back(o->intersect(shadowRay));
		}

		for (BVHNode *b : _bvh)
		{
			shadowIntersections.push_back(
				b->intersect(shadowRay, b).intersect(shadowRay));
		}

		Intersection closestShadowIntersection;
		bool inShadow = false;

		for (std::vector<Intersection> i : shadowIntersections)
		{
			if (!i.empty())
			{
				inShadow = true;
				for (Intersection j : i)
					if (closestShadowIntersection._distance == -1.f
						|| j._distance < closestShadowIntersection._distance)
						closestShadowIntersection = j;
			}
		}

		if (inShadow
			&& closestShadowIntersection._distance < length(lightVector)
			&& closestShadowIntersection._object != currentObject)
		{
			color =glm::min(color,color/(1.3f * light->getIntensity(closestShadowIntersection._distance)));
			return 1;
		}
		return 0;
	}

	/*
	 * Fonction permettant de récupérer la couleur de l'objet en un point donnée
	 * Utilise la fonction getColor pour effectuer ceci
	 */

	Vec3f LightMaterial::shade(const Ray &ray, const Intersection &hitPoint)
	{
		return getColor(ray, hitPoint, 0);
	}

	/*
	 * Fonction permettant de récupérer la couleur de l'objet en un point donnée
	 * La fonction calcule la couleur de réflection, celle de réfraction,
	 * ainsi que la couleur diffuse de l'objet et la couleur spéculaire
	 * au travers de la BRDF et du matériau de l'objet
	 */

	Vec3f LightMaterial::getColor(const Ray &ray, const Intersection &hitpoint,
								  int loop)
	{
		AObject3D *currentObject = hitpoint._object;
		Vec3f currentObjectColor = currentObject->getf0();

		Vec3f rflColor = VEC3F_ZERO;
		Vec3f refrColor = VEC3F_ZERO;

		if (loop <= _limit)
		{
			loop++;

			Vec3f normale = hitpoint._normale;			
			
			float etatAir = 1.f;														// Indice de réfraction dans l'air
			float refractionIndex= etatAir / currentObject->getRefractionIndex();		// Si on est à l'intérieur del'objet et qu'on en ressort	
																						// On inverse la normale et  l'indice de réfraction																		
			if (glm::dot(ray.getDirection(), normale) > 0)

			{
				normale = -normale;
				refractionIndex = currentObject->getRefractionIndex();
			}

			float rflA = currentObject->getReflectionAmount();
			float rfrA = currentObject->getRefractionAmount();
			if (rflA > 0.f)															//Si l'objet reflète
			{
				std::vector<std::vector<Intersection>> intersectionsReflection;

				Ray reflectedRay(
					hitpoint._position,
					glm::normalize(glm::reflect(ray.getDirection(), normale)));


				for (AObject3D *o : _objects)									//Ici on va tester les intersections entre le rayon réfléchi et les objets
				{
					intersectionsReflection.push_back(
						o->intersect(reflectedRay));
				}

				for (BVHNode *b : _bvh)											 // Pareil pour les BVH
				{
					intersectionsReflection.push_back(b->intersect(reflectedRay, b).intersect(reflectedRay));
				}

				bool rflRayHasIntersected = false;
				Intersection closestReflectIntersection;

				for (std::vector<Intersection> i : intersectionsReflection)		// Recherche de l'intersection la plus proche entre le rayon
																				// réfléchi et les objets de la scène, si elle existe
				{ 
					if (!i.empty())
					{
						rflRayHasIntersected = true;
						for (Intersection j : i)
						{
							if (closestReflectIntersection._distance == -1.f
								|| j._distance
									   < closestReflectIntersection._distance)
							{
								closestReflectIntersection = j;
							}
						}
					}
				}

				if (rflRayHasIntersected)										// Si on a une intersection de reflection, on va réeffectuer un calcul de couleur avec
				{																// cette fois-ci comme rayon primaire le rayon
																				// réfléchi et l' intersection de reflection
					rflColor
						+= getColor(reflectedRay, closestReflectIntersection,	// on obtient récursivement la couleur de réflexion
									loop);		
					
				}
				else
				{
					rflColor += VEC3F_ZERO;
				}

			}
				 
			if (rfrA > 0.f)														// Si l'objet refracte
			{
				std::vector<std::vector<Intersection>> intersectionsRefraction;


				Ray refractedRay(
					hitpoint._position,	glm::normalize(glm::refract(ray.getDirection(),	normale,	// Calcul du rayon réfracté entre la normale à
					refractionIndex)));																// la position et le rayon primaire

				for (AObject3D *o : _objects)														// Ici on va tester les intersections entre le
																									// rayon refracté et tous les objets de la scène
				{														
					intersectionsRefraction.push_back(o->intersect(refractedRay));
				}

				for (BVHNode *b : _bvh)																// Pareil pour les BVH
				{
					intersectionsRefraction.push_back(b->intersect(refractedRay, b).intersect(refractedRay));
				}


				bool rfrRayHasIntersected = false;
				Intersection closestRefractIntersection;

				for (std::vector<Intersection> i : intersectionsRefraction)					// Recherche de l'intersection la plus proche entre le rayon																			
				{																			// réfracté et les objets de la scène, si elle existe
					if (!i.empty())
					{
						rfrRayHasIntersected = true;
						for (Intersection j : i)
							if (closestRefractIntersection._distance == -1.f
								|| j._distance
									   < closestRefractIntersection._distance)
								closestRefractIntersection = j;
						;
					}
				}
																							// Si on a une intersection de réfraction, on va
																							// réeffectuer un calcul de couleur avec cette
																							// fois-ci comme rayon primaire
				if (rfrRayHasIntersected)													// le rayon réfracté et l' intersection de réfraction									  

				{
					refrColor += getColor(refractedRay, closestRefractIntersection,	loop);	// on obtient récursivement la couleur de réfraction
				}
				else
				{
					refrColor += VEC3F_ZERO;
				}
			}

			//printf("%f %f %f \n", refrColor.x,refrColor.y, refrColor.z);
		
			Vec3f albedo = currentObject->getColorMaterial()->shade(ray, hitpoint);      //Albedo de l'objet

			Vec3f color = VEC3F_ZERO;
			Vec3f specular = VEC3F_ZERO;

			for (Light *light : _lights)												//Ici, on va calculer la couleur de l'objet à l'aide de la
			{																			//BRDF et des différentes lumières de la scène
				Vec3f lightVector = light->getPosition() - hitpoint._position;

				Vec3f normalizedLightVector = glm::normalize(lightVector);				//Vecteur lumière normalisé

				Vec3f normalizedViewVector												//Vecteur de vue normalisé
					= glm::normalize(_camera.getPosition() - hitpoint._position);

				float lightIntensity = light->getIntensity(length(lightVector));		//Intensité de la lumière au point

				Vec3f diffuse = albedo / (float)M_PI;									//Couleur diffuse


				float cosine
					= fmaxf(0.f, glm::dot(normalizedLightVector, normale));				//Cosine entre le vecteur de lumière et la normale

				float currentObjectRugosity = currentObject->getRugosity();				//Rugosité de l'objet

				Vec3f f0 = currentObject->getf0();										//Coefficient de fresnel de l'objet

				Vec3f fresnelDiff = _brdf.fresnelTerm(									//Terme de fresnel diffus
					f0, cosine);

				specular
					= _brdf.brdfTerm(normalizedLightVector, normalizedViewVector,		//Couleur spéculaire de l'objet 
									 normale, currentObjectRugosity, f0);

		
				color += lightIntensity * cosine										//On combine les couleurs spéculaires, diffuses et 
						 * glm::mix(diffuse,  specular + albedo, fresnelDiff);			//l'albédo pour cette lumière

			}

			Vec3f finalColor;															//calcul de la couleur finale
			finalColor.x																//on combine la couleur de l'objet, celle de la réfraction 
				= glm::min(rfrA * refrColor.x + rflA * rflColor.x + color.x, 1.0f);		//et de la réflection

			finalColor.y
				= glm::min(rfrA * refrColor.y + rflA * rflColor.y + color.y, 1.0f);

			finalColor.z
				= glm::min(rfrA * refrColor.z + rflA * rflColor.z + color.z, 1.0f);

			int shadowed=0;																//On calcule l'ombre de la couleur finale
			for (Light* light : _lights){	
				shadowed += calculateShadow(hitpoint, light, finalColor,
											   currentObject);			
			}
			if (shadowed!=0)
				return finalColor / float(shadowed);

			return finalColor;
			}
		else 
			return Vec3f(0.0f);															
	} 

	/*
	 * Setters des variables de l'objet
	 */

	void LightMaterial::setObjects(std::vector<AObject3D *> &objects)
	{
		_objects = objects;
	}

	void LightMaterial::setBVH(std::vector<BVHNode *> &bvh)
	{
		_bvh = bvh;
	}


} // namespace ISICG_ISIR