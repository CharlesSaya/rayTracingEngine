#include "triangleMesh.hpp"

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace ISICG_ISIR
{

	/*
	 * Constructeur de classe
	 */

	
	TriangleMesh::TriangleMesh(const std::vector<MeshTriangle> &triangles,
							   const std::vector<Vec3f> &vertices,
							   const std::vector<Vec3f> &normales)
		: _triangles(triangles), _vertices(vertices), _normales(normales)
	{
	}

	/*
	 * Constructeur de classe
	 */

	TriangleMesh::TriangleMesh(const float reflectionAmount,
							   const float refractionAmount,
							   const float refractionIndex,
							   const float rugosity,
							   const Vec3f &f0)
	{
		_reflectionAmount = reflectionAmount;
		_refractionAmount = refractionAmount;
		_refractionIndex = refractionIndex;
		_rugosity = rugosity;
		_f0 = f0;
	}

	/*
	 * Fonction récupérant la normale d'un triangle
	 */

	const Vec3f &TriangleMesh::getTriangleNormal(const MeshTriangle &t)
	{
		Vec3f v0 = _vertices[t._v0];
		Vec3f v1 = _vertices[t._v1];
		Vec3f v2 = _vertices[t._v2];

		return glm::normalize(glm::cross((v1 - v0), (v2 - v0)));
	}

	/*
	 * Fonction récupérant la normale interpolée d'un sommet.
	 * L'interpolation est faite avec les normales de chaque triangles
	 * incidents au sommet.
	 */

	const Vec3f &TriangleMesh::getVerticeNormal(const uint pt)
	{
		std::vector<MeshTriangle> adjacentTriangles;

		for (MeshTriangle triangle : _triangles)				//On récupère les triangles incidents à notre sommet
		{
			for (int j = 0; j < 3; j++)
			{
				if (_vertices[triangle._v[j]] == _vertices[pt])
				{
					adjacentTriangles.push_back(triangle);
				}
			}
		}

		Vec3f normale = VEC3F_ZERO;
		for (MeshTriangle t : adjacentTriangles)				//On accumule les normales de chaque face et récupère la moyenne de
																// de celles-ci puis on normalize le résultat
		{
			normale += getTriangleNormal(t);
		}

		// printf("%d\n", adjacentTriangles.size());
		return Vec3f(glm::normalize(normale / (float)adjacentTriangles.size()));
	}



	// Load a mesh with assimp
	void TriangleMesh::load(const std::string &path)
	{
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene *const scene
			= importer.ReadFile(path.c_str(), aiProcess_Triangulate);

		if (scene == nullptr)
		{
			throw std::runtime_error("Cannot import file: " + path);
		}
		const uint nbMeshes = scene->mNumMeshes;
		uint nbTriangles = 0;
		uint nbVertices = 0;

		for (uint i = 0; i < nbMeshes; ++i)
		{
			const aiMesh *const mesh = scene->mMeshes[i];
			nbTriangles += mesh->mNumFaces;
			nbVertices += mesh->mNumVertices;
		}
		_triangles.resize(nbTriangles);
		_vertices.resize(nbVertices);
		_normales.resize(nbVertices);

		uint currentTriangle = 0;
		uint currentVertex = 0;

		for (uint m = 0; m < nbMeshes; ++m)
		{
			const aiMesh *const mesh = scene->mMeshes[m];
			const aiMaterial *const material
				= scene->mMaterials[mesh->mMaterialIndex];

			for (uint f = 0; f < mesh->mNumFaces; ++f, ++currentTriangle)
			{
				const aiFace face = mesh->mFaces[f];

				MeshTriangle &tri = _triangles[currentTriangle];

				// triangulated ! :-)
				for (uint v = 0; v < 3; ++v)
				{
					const uint idV = face.mIndices[v];
					tri._v[v] = idV;
				}
			}

			for (uint v = 0; v < mesh->mNumVertices; ++v, ++currentVertex)
			{
				Vec3f &vertex = _vertices[currentVertex];
				vertex.x = mesh->mVertices[v].x;
				vertex.y = mesh->mVertices[v].y;
				vertex.z = mesh->mVertices[v].z;
			}
			currentVertex = 0;
			for (uint v = 0; v < mesh->mNumVertices; ++v, ++currentVertex)   //Ici, on récupère les normales de chaque sommet que l'on stocke
			{																 // dans un tableau
				Vec3f &normal = _normales[currentVertex];
				Vec3f n = getVerticeNormal(v);
				normal.x = n.x;
				normal.y = n.y;
				normal.z = n.z;
			}
		}

		std::cout << "Loaded: " << std::endl
				  << "- nb faces: " << _triangles.size() << std::endl
				  << "- nb vertices: " << _vertices.size() << std::endl
				  << "- nb normales: " << _normales.size() << std::endl;
	}

	/*
	 * Fonction d'intersection. Ici on va s'aider de la fonction rayTriangleIntersection()
	 * pour pourvoir récupérer calculer si il y a intersection avec le rayon.
	 */


	std::vector<Intersection> TriangleMesh::intersect(const Ray &ray)
	{
		std::vector<Intersection> intersections;
		for (int i = 0; i < _triangles.size(); i++)
		{
			Intersection inter;
			if (rayTriangleIntersection(ray, _triangles[i], inter))
			{
				intersections.push_back(inter);
			}
		}
		return intersections;
	}

	/*
	 * Fonction calculant l'intersection avec un rayon si elle existe et retourne
	 * un booléen selon le résultat. 
	 * C'est dans cette fonction que l'interpolation des normales du triangle s'effectue
	 * si on trouve une intersection avec celui_ci.
	 */

	bool TriangleMesh::rayTriangleIntersection(const Ray &ray,
											   const MeshTriangle &triangle,
											   Intersection &hitPoint)
	{
		float epsilon = 10e-6f;

		Vec3f v0 = _vertices[triangle._v0];
		Vec3f v1 = _vertices[triangle._v1];
		Vec3f v2 = _vertices[triangle._v2];

		Vec3f v0v1 = v1 - v0;
		Vec3f v0v2 = v2 - v0;

		Vec3f pvec = glm::cross(ray.getDirection(), v0v2);

		float det = glm::dot(v0v1, pvec);

		if (det>-epsilon && det<epsilon) // ray is parallel to triangle
			return false;

		float invDet = 1.f / det;

		Vec3f tVec = ray.getOrigin() - v0;

		float detA = glm::dot(tVec, pvec);

		float u = invDet * detA;

		if (u < 0.f || u > 1.f)
			return false;

		Vec3f qVec = glm::cross(tVec, v0v1);

		float detB = glm::dot(ray.getDirection(), qVec);

		float v = detB * invDet;

		if (v < 0.f || (u + v) > 1.f)
			return false;

		float detC = glm::dot(v0v2, qVec);

		float t = detC * invDet;
		if (t > epsilon)											//On a une intersection avec le triangle en question
		{
			Vec3f hit = ray.getOrigin() + t * ray.getDirection();

			Vec3f normaleV0 = _normales[triangle._v0];				//Ici, on va interpoler les normales du triangle pour lisser l'objet
			Vec3f normaleV1 = _normales[triangle._v1];				//lors des calculs d'éclairage et de couleur
			Vec3f normaleV2 = _normales[triangle._v2];
				
			float w1 = (v1.y - v2.y) * (hit.x - v2.x)
					   + (v2.x - v1.x) * (hit.y - v2.y);

			float dw1
				= (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);

			float w2 = (v2.y - v0.y) * (hit.x - v2.x)
					   + (v0.x - v2.x) * (hit.y - v2.y);

			float dw2
				= (v1.y - v2.y) * (v0.x - v2.x) + (v2.x - v1.x) * (v0.y - v2.y);

			w1 /= dw1;
			w2 /= dw2;

			float w3 = 1 - w1 - w2;

			Vec3f normale = glm::normalize(
				(w1 * normaleV0 + w2 * normaleV1 + w3 * normaleV2));

			
			hitPoint = Intersection(t,hit + normale * (float)10e-2, normale, this);
			return true;
		}
		return false;
	}

	/*
 	 * Getters des variables de la classe
	 */

	std::vector<MeshTriangle> &TriangleMesh::getTriangles()
	{
		return _triangles;
	}

	std::vector<Vec3f> &TriangleMesh::getVertices()
	{
		return _vertices;
	}

	std::vector<Vec3f> &TriangleMesh::getNormales()
	{
		return _normales;
	}

} // namespace ISICG_ISIR
