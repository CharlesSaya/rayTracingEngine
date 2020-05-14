#ifndef __ISICG_ISIR_TRIANGLE_MESH__
#define __ISICG_ISIR_TRIANGLE_MESH__

#include <vector>

#include "assimp/mesh.h"

#include "defines.hpp"

#include "aObject3D.hpp"

namespace ISICG_ISIR
{
	/*
	 * Structure représentant un triangle
	 */

	struct MeshTriangle
	{
		// vertices ids in TriangleMesh::_vertices
		union {
			struct
			{
				uint _v0, _v1, _v2;
			};
			uint _v[3];
		};
	};

	/*
	 * Classe représentant les Mesh 3D
	 */

	class TriangleMesh : public AObject3D
	{
	public:
		TriangleMesh() = default;

		TriangleMesh(const std::vector<MeshTriangle> &triangles,
					 const std::vector<Vec3f> &vertices,
					 const std::vector<Vec3f> &normales);

		TriangleMesh(const float reflectionAmount, const float refractionAmount,
					 const float refractionIndex, const float rugosityn,
					 const Vec3f &f0);

		void load(const std::string &path);

		std::vector<Intersection> intersect(const Ray &ray) override;

		bool rayTriangleIntersection(const Ray &ray,
									 const MeshTriangle &triangle,
									 Intersection &hitPoint);

		std::vector<MeshTriangle> &getTriangles();

		std::vector<Vec3f> &getVertices();

		std::vector<Vec3f> &getNormales();

		const Vec3f& getTriangleNormal(const MeshTriangle &t);

		const Vec3f& getVerticeNormal(const uint pt);

	private:
		std::vector<MeshTriangle> _triangles; // triangles du mesh
		std::vector<Vec3f> _vertices;		  // sommets du mesh
		std::vector<Vec3f> _normales;		  // normales de chaque sommet
	};
} // namespace ISICG_ISIR

#endif // __ISICG_ISIR_TRIANGLE_MESH__
