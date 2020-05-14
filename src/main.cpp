#include "3D/implicitCylinder.hpp"
#include "3D/implicitSphere.hpp"
#include "3D/implicitSurface.hpp"
#include "3D/plan.hpp"
#include "3D/sphere.hpp"
#include "3D/triangleMesh.hpp"
#include "BVH/bvhnode.hpp"
#include "camera.hpp"
#include "chrono.hpp"
#include "defines.hpp"
#include "image.hpp"
#include "light/light.hpp"
#include "light/pointLight.hpp"
#include "materials/checkeredMaterial.hpp"
#include "materials/flatColorMaterial.hpp"
#include "materials/lightMaterial.hpp"
#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <random>
#include <string>

#define AIR_INDEX 1.f
#define WATER_INDEX 1.3f
#define GLASS_INDEX 1.5f
#define DIAMOND_INDEX 1.7f

namespace ISICG_ISIR
{
	/**
	 * Get the sub-pixels used to launch rays through them when AntiAliasing is
	 *activated
	 **/

	std::vector<Vec2f> superSampling(const Vec2f &pixel, float power,
									 float width, float height)
	{
		std::vector<Vec2f> pixels;
		if (power == 0.f)
		{
			pixels.push_back(Vec2f(pixel.x / width, pixel.y / height));
			return pixels;
		}

		for (int j = 0; j <= power; j++)
		{
			for (int i = 0; i <= power; i++)
			{
				pixels.push_back(Vec2f((pixel.x + (i / power)) / width,
									   (pixel.y + (j / power)) / height));
			}
		}
		return pixels;
	}

	int main(int argc, char **argv)
	{
		if (argc != 4)
		{
			throw std::runtime_error(
				"Invalid program argument: must be launched with <imageWidth> "
				"<imageHeight> <imageName>\n e.g. 800 600 results.jpg");
		}

		// get arguments
		const std::string name(argv[1]);
		const uint width = 960;
		const uint height = 540;
		ImageJPG image(width, height);

		// CAMERA
		// ----------------------------------------------------------------------------------------------------------------

		Camera camera(width, height, 1.f);

		// LIGHTS
		// ----------------------------------------------------------------------------------------------------------------

		Light ambientLight(Vec3f(1.0, 1.0, 1.0), Vec3f(-.5f, 1.5f, 1.f));
		PointLight pointLight(Vec3f(1.0, 1.0, 1.0), Vec3f(.0f, .4f, 0.3f),
							  0.7f);
		PointLight pointLight2(Vec3f(1.0, 1.0, 1.0), Vec3f(-.8f, 5.f, 7.9f),
							   0.002f);
		PointLight pointLight3(Vec3f(1.0, 1.0, 1.0), Vec3f(.8f, 5.f, 7.9f),
							   0.002f);

		std::vector<Light *> lights;
		lights.push_back(&pointLight);
		

		// OBJECTS ALBEDO COLORS
		// -------------------------------------------------------------------------------------------------

		FlatColorMaterial sphereColor(Vec3f(1.0, 0.0, 0.0));	
		FlatColorMaterial sphereColor2(Vec3f(0.98, 0.97, 0.95)); 
		FlatColorMaterial sphereColor3(Vec3f(.0, 0.5, 0.5)); 
		FlatColorMaterial sphereColor4(Vec3f(0.98, 0.82, 0.76)); 
		FlatColorMaterial sphereColor5(Vec3f(0.5, 0.0, 0.5));	 
		FlatColorMaterial sphereColor6(Vec3f(0., 0., 0.09));	 
		FlatColorMaterial sphereColor7(Vec3f(1.00, 0.86, 0.57)); 
		FlatColorMaterial sphereColor8(Vec3f(1.0));				 
		FlatColorMaterial sphereColor9(Vec3f(.1f));

		FlatColorMaterial planColor(Vec3f(1.f, 0.86f, 0.57f));
		FlatColorMaterial planColor2(Vec3f(0., 0., 0.09));
		FlatColorMaterial planColorLat(Vec3f(0.1f));

		FlatColorMaterial cylColor(Vec3f(0.1f));

		FlatColorMaterial beybladeColor(Vec3f(1.00, 0.86, 0.57));

		FlatColorMaterial meshAlbedo(Vec3f(0.1));

		// LIGHT MATERIAL
		// ----------------------------------------------------------------------------------------------------------

		LightMaterial lightMaterial(lights, camera, 3);

		// SCENE OBJECTS
		// -----------------------------------------------------------------------------------------------------------

		// BVH  (Montant de refelxion, Montant de refraction, Indice de refraction, rugosité, Coefficient de Fresnel)
		// ----------------------------------------------------------------------------------------------------------------
		TriangleMesh bunny(1.0f, 0.f, AIR_INDEX, 0.02, Vec3f(0.21f));
		bunny.load("data\\obj\\darkrai.obj");
		bunny.setMaterial(&lightMaterial);
		bunny.setColorMaterial(&meshAlbedo);

		std::cout << "Begin building BVH" << std::endl;

		Chrono chrono;
		chrono.start();
		Box box;
		BVHNode *bunnyBVH = new BVHNode(bunny, box);
		chrono.stop();
		std::cout << "BVH Construction was done in  " << chrono.elapsedTime()
				  << "s " << std::endl;

		// SPHERE (Centre,Rayon, Montant de refelxion, Montant de refraction, Indice de refraction, rugosité, Coefficient de Fresnel)
		// ----------------------------------------------------------------------------------------------------------------

		Sphere sphere(Vec3f(0.35f, -0.2f, 0.9f), 0.035f, 1.0f, 0.f, AIR_INDEX,
					  0.5, Vec3f(0.98, 0.97, 0.95));
		sphere.setMaterial(&lightMaterial);
		sphere.setColorMaterial(&sphereColor);

		Sphere sphere2(Vec3f(-.25f, -0.1f, 0.92f), 0.035f, 0.5f, 0.f, AIR_INDEX,
					   0.5,
					   Vec3f(0.98, 0.97, 0.95));
		sphere2.setMaterial(&lightMaterial);
		sphere2.setColorMaterial(&sphereColor2);

		Sphere sphere3(Vec3f(-0.35f, -0.f, 1.03f), 0.035f, 1.0f, 0.f, AIR_INDEX,
					   0.1, Vec3f(0.21f));
		sphere3.setMaterial(&lightMaterial);
		sphere3.setColorMaterial(&sphereColor3);

		Sphere sphere4(Vec3f(-0.1f, -0.3f, 0.7f), 0.035f, 0.5f, 0.f, AIR_INDEX,
					   0.1, Vec3f(0.98, 0.82, 0.76));
		sphere4.setMaterial(&lightMaterial);
		sphere4.setColorMaterial(&sphereColor4);

		Sphere sphere5(Vec3f(0.25f, .2f, 1.2f), 0.035f, 0.f, 0.f, AIR_INDEX,
					   0.8, Vec3f(1.00, 0.86, 0.57));
		sphere5.setMaterial(&lightMaterial);
		sphere5.setColorMaterial(&sphereColor5);

		Sphere sphere6(Vec3f(0.36f, .13f, 1.3f), 0.035f, 0.f, 0.5f,
					   DIAMOND_INDEX, 0.02, Vec3f(0.21f));
		sphere6.setMaterial(&lightMaterial);
		sphere6.setColorMaterial(&sphereColor6);

		Sphere sphere7(Vec3f(0.42f, .02f, 1.12f), 0.035f, 0.f, 0.0f, AIR_INDEX,
					   0.02, Vec3f(1.00, 0.86, 0.57));
		sphere7.setMaterial(&lightMaterial);
		sphere7.setColorMaterial(&sphereColor7);

		Sphere sphere8(Vec3f(0.46f, -0.12f, 1.09f), 0.035f, 0.f, 0.5f,
					   DIAMOND_INDEX, 0.02, Vec3f(0.21f));
		sphere8.setMaterial(&lightMaterial);
		sphere8.setColorMaterial(&sphereColor8);

		Sphere sphere9(Vec3f(-0.35f, -0.2f, 0.9f), 0.035f, 1.f, 0.f, AIR_INDEX,
					   0.5, Vec3f(0.98, 0.97, 0.95));
		sphere9.setMaterial(&lightMaterial);
		sphere9.setColorMaterial(&sphereColor);

		Sphere sphere10(Vec3f(.25f, -0.1f, 0.92f), 0.035f, 0.5f, 0.f, AIR_INDEX,
						0.5,
						Vec3f(0.98, 0.97, 0.95));
		sphere10.setMaterial(&lightMaterial);
		sphere10.setColorMaterial(&sphereColor2);

		Sphere sphere11(Vec3f(0.35f, -0.f, 1.03f), 0.035f, 1.0f, 0.f, AIR_INDEX,
						0.1, Vec3f(0.21f));
		sphere11.setMaterial(&lightMaterial);
		sphere11.setColorMaterial(&sphereColor3);

		Sphere sphere12(Vec3f(0.1f, -0.3f, 0.7f), 0.035f, .5f, 0.f, AIR_INDEX,
						0.1, Vec3f(0.98, 0.82, 0.76));
		sphere12.setMaterial(&lightMaterial);
		sphere12.setColorMaterial(&sphereColor4);

		Sphere sphere13(Vec3f(-0.25f, .2f, 1.2f), 0.035f, 0.f, 0.f, AIR_INDEX,
						0.8, Vec3f(1.00, 0.86, 0.57));
		sphere13.setMaterial(&lightMaterial);
		sphere13.setColorMaterial(&sphereColor5);

		Sphere sphere14(Vec3f(-0.36f, .13f, 1.3f), 0.035f, 0.f, 0.5f,
						DIAMOND_INDEX, 0.02, Vec3f(0.21f));
		sphere14.setMaterial(&lightMaterial);
		sphere14.setColorMaterial(&sphereColor6);

		Sphere sphere15(Vec3f(-0.42f, .02f, 1.12f), 0.035f, 0.f, 0.0f,
						AIR_INDEX, 0.02, Vec3f(1.00, 0.86, 0.57));
		sphere15.setMaterial(&lightMaterial);
		sphere15.setColorMaterial(&sphereColor7);

		Sphere sphere16(Vec3f(-0.46f, -0.12f, 1.09f), 0.035f, 1.f, 0.f,
						AIR_INDEX, 0.02, Vec3f(0.21f));
		sphere16.setMaterial(&lightMaterial);
		sphere16.setColorMaterial(&sphereColor8);

		Sphere sphere17(Vec3f(-0.f, 0.5f, 1.5f), 0.2f, 0.f, 1.f, DIAMOND_INDEX,
						0.02, Vec3f(0.21f));
		sphere17.setMaterial(&lightMaterial);
		sphere17.setColorMaterial(&sphereColor9);

		// PLANES (Normale,Point du plan, Montant de refelxion, Montant de refraction, Indice de refraction, rugosité, Coefficient de Fresnel)
		// ----------------------------------------------------------------------------------------------------------------

		Plan plan(Vec3f(0.f, 1.f, 0.f), Vec3f(0.f, -0.8f, 0.f), 1.0f, 0.f,
				  AIR_INDEX, 0.2, Vec3f(0.21));
		plan.setMaterial(&lightMaterial);
		plan.setColorMaterial(&planColor);

		Plan plan2(Vec3f(0.f, 0.f, -1.f), Vec3f(0.f, 0.7f, 8.f), 1.0f, 0.f,
				   AIR_INDEX, 0.1, Vec3f(0.77, 0.78, 0.78));
		plan2.setMaterial(&lightMaterial);
		plan2.setColorMaterial(&planColor2);

		Plan plan4(Vec3f(1.f, 0.f, 0.f), Vec3f(-5.f, 0.f, 0.f), 1.0f, 0.f,
				   AIR_INDEX, 0.1, Vec3f(0.77, 0.78, 0.78));
		plan4.setMaterial(&lightMaterial);
		plan4.setColorMaterial(&planColorLat);

		Plan plan5(Vec3f(-1.f, 0.f, 0.f), Vec3f(5.f, 0.f, -0.f), 1.0f, 0.f,
				   AIR_INDEX, 0.1, Vec3f(0.77, 0.78, 0.78));
		plan5.setMaterial(&lightMaterial);
		plan5.setColorMaterial(&planColorLat);

		// IMPLICIT CYLINDRE (Centre,Rayon, Montant de refelxion, Montant de refraction, Indice de refraction, rugosité, Coefficient de Fresnel)
		// ----------------------------------------------------------------------------------------------------------------

		ImplicitCylinder iCyl(Vec3f(-2.5f, 0.f, 5.0f), 0.3f, 0.5f, 0.f,
							  AIR_INDEX, 0.2, Vec3f(0.77, 0.78, 0.78));
		iCyl.setMaterial(&lightMaterial);
		iCyl.setColorMaterial(&cylColor);

		ImplicitCylinder iCyl2(Vec3f(2.5f, 0.f, 5.0f), 0.3f, 0.5f, 0.f,
							   AIR_INDEX, 0.2, Vec3f(0.77, 0.78, 0.78));
		iCyl2.setMaterial(&lightMaterial);
		iCyl2.setColorMaterial(&cylColor);


		// IMPLICIT CYLINDRE (Centre,Montant de refelxion, Montant de refraction, Indice de refraction, rugosité, Coefficient de Fresnel)
		// ----------------------------------------------------------------------------------------------------------------

		ImplicitSurface beyblade1(Vec3f(-1.4f, 0.f, 2.f), 0.f, 0.f, 0.f, 0.02,
								  Vec3f(0.21));
		beyblade1.setMaterial(&lightMaterial);
		beyblade1.setColorMaterial(&beybladeColor);

		ImplicitSurface beyblade2(Vec3f(1.4f, 0.f, 2.f), 0.0f, 0.f, 0.f, 0.02,
								  Vec3f(0.21));
		beyblade2.setMaterial(&lightMaterial);
		beyblade2.setColorMaterial(&beybladeColor);

		// PLACE OBJECTS IN QUEUE
		// ----------------------------------------------------------------------------------------------------------------

		std::vector<AObject3D *> objects;
		objects.emplace_back(&sphere);
		objects.emplace_back(&sphere2);
		objects.emplace_back(&sphere3);
		objects.emplace_back(&sphere4);
		objects.emplace_back(&sphere5);
		objects.emplace_back(&sphere6);
		objects.emplace_back(&sphere7);
		objects.emplace_back(&sphere8);
		objects.emplace_back(&sphere9);
		objects.emplace_back(&sphere10);
		objects.emplace_back(&sphere11);
		objects.emplace_back(&sphere12);
		objects.emplace_back(&sphere13);
		objects.emplace_back(&sphere14);
		objects.emplace_back(&sphere15);
		objects.emplace_back(&sphere16);
		objects.emplace_back(&sphere17);
		objects.emplace_back(&plan);
		objects.emplace_back(&plan2);
		objects.emplace_back(&plan4);
		objects.emplace_back(&plan5);
		objects.emplace_back(&iCyl);
		objects.emplace_back(&iCyl2);
		objects.emplace_back(&beyblade1);
		objects.emplace_back(&beyblade2);

		std::vector<BVHNode *> bvhNodes;
		//bvhNodes.emplace_back(bunnyBVH);

		// SET OBJECTS IN THE LIGHT MATERIAL USED TO GET OBJECTS COLORS
		//----------------------------------------------------------------------------------------------------------------

		lightMaterial.setBVH(bvhNodes);
		lightMaterial.setObjects(objects);

		// INTERSECTIONS----------------------------------------------------------------------------------------------------------------

		std::vector<std::vector<Intersection>> intersectedObjects;
		Ray ray;

		// ANTI ALIASING POWER
		// ----------------------------------------------------------------------------------------------------------------
		float AAPower = 0.f;

		// random----------------------------------------------------------------------------------------------------------------
		std::random_device rd;
		std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
		std::uniform_real_distribution<float> dis(0.f, 1.f);

		chrono.start();

		for (int h = 0; h < height; ++h) // On commence le rendering de l'image
		{
			for (int w = 0; w < width; ++w)
			{
				Vec3f AAcolor = VEC3F_ZERO;
				std::vector<Vec2f> pixels = superSampling(Vec2f(((float)w), ((float)h)), AAPower, float(width),	float(height)); // si on a activé l'anti aliasing, on
																																// récupère les sous-pixels du pixel
				for (Vec2f pixel :
					 pixels) // pour chacun de ces pixels on va lancer un rayon
				{
					ray = camera.generateRay(pixel);

					for (int o = 0; o < objects.size(); ++o) // On recherche les intersections entre le rayon et les objets
					{
						intersectedObjects.push_back(
							objects[o]->intersect(ray));
					}

					for (int o = 0; o < bvhNodes.size(); ++o) // Pareil pour les BVH
					{
						intersectedObjects.push_back(
							bvhNodes[o]
								->intersect(ray, bvhNodes[o])
								.intersect(ray));
					}

					Intersection nearestIntersection;
					bool isIntersecting = false;

					for (std::vector<Intersection> i : intersectedObjects) // On recherche l'intersection la
											 // plus proche si il y en a une
					{
						if (i.size() != 0)
						{
							isIntersecting = true;
							for (Intersection j : i)
								if (nearestIntersection._distance == -1.f || j._distance  < nearestIntersection._distance)
								{
									nearestIntersection = j;
								}
						}
					}

					if (isIntersecting)											 // Si on intersecte quelque chose, on va calculer la couleur de cet objet
					{															 // Dans le cas de l'antialiasing, on va ajouter chacune  des couleurs
					
						AObject3D *intersectedObject                             // samplés par les rayons lancés dans les sous-pixels du pixel actuel
							= nearestIntersection._object;						 // puis on fait une moyenne de ces couleurs.
						AAcolor += intersectedObject->getMaterial()->shade(
							ray, nearestIntersection);
					}
					else
						AAcolor += Vec3f(0.f, 0.75f,1.f);						 // sinon on renvoie la couleur du fond

					intersectedObjects.clear();
				}

				if (AAPower != 0.f)
					image.setPixel(
						w, h,
						Vec4f(AAcolor / ((AAPower + 1) * (AAPower + 1)), 1));	// on colore le pixel avec la moyenne des couleurs samplés
				else
					image.setPixel(
						w, h, Vec4f(AAcolor, 1)); // ou simplement la couleur
			}

			float progress
				= (float)(h) / (float)(height); // Barre de progression
			float barLength = 30;
			int pos = progress * barLength;

			std::cout << "[";
			for (int i = 0; i != barLength; ++i)
			{
				if (i <= pos)
					std::cout << "#";
				else
					std::cout << " ";
			}
			std::cout << "]\r";
		}
		chrono.stop();
		std::cout << "Rendering done. Image computed in "
				  << chrono.elapsedTime() << "s (" << image.getWidth() << "x"
				  << image.getHeight() << ")" << std::endl;

		std::cout << "Save image as: " << name << std::endl;
		image.save(name);

		delete bunnyBVH;

		return EXIT_SUCCESS;
	}
} // namespace ISICG_ISIR

int main(int argc, char **argv)
{
	try
	{
		return ISICG_ISIR::main(argc, argv);
	}
	catch (const std::exception &e)
	{
		std::cerr << "Exception caught !" << std::endl << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}
