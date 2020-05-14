#ifndef __ISICG_ISIR_BVH_NODE___
#define __ISICG_ISIR_BVH_NODE___

#include "3D/intersection.hpp"
#include "3D/ray.hpp"
#include "3D/triangleMesh.hpp"
#include "box.hpp"
#include "defines.hpp"
#include <vector>

namespace ISICG_ISIR
{
	
	/*
	 * Classe implémentant la méthode de BVH SAH
	 */


	class BVHNode
	{
	public:
		BVHNode() = default;

		BVHNode(TriangleMesh &triangleMesh, const Box &box);

		void findBoundingBox();

		void sortTriangles(std::vector<MeshTriangle> &triangles,
						   const std::vector<Vec3f> &vertices, const int axis);

		const Vec3f &barycentreTriangle(const MeshTriangle &m,
								 const std::vector<Vec3f> &vertices);

		std::vector<MeshTriangle> getIntersectedTriangles(const Ray &ray,
														  BVHNode *node);

		TriangleMesh intersect(const Ray &ray, BVHNode *root);

		~BVHNode();

	private:
		Box _box;												//Boite englobant les triangles
		BVHNode *_lchild = nullptr, *_rchild = nullptr;			//Fils de la classe
		TriangleMesh _triangleMesh;								//TriangleMesh contenant les triangles contenus dans la boite actuelle.
	};

} // namespace ISICG_ISIR

#endif //__ISICG_ISIR_BVH_NODE___