#include "bvhnode.hpp"
#include "defines.hpp"
#include <map>
#include <omp.h>

namespace ISICG_ISIR
{
	
	/*
	 * Constructeur de classe récursif
	 * Permet de construire l'arbre BVH que l'on va parcourir
	 * pour trouver les intersections entre le mesh et le rayon
	 */

	BVHNode::BVHNode(TriangleMesh &triangleMesh, const Box &box)
	{
		std::vector<MeshTriangle> &triangles = triangleMesh.getTriangles();
		std::vector<Vec3f> &vertices = triangleMesh.getVertices();
		std::vector<Vec3f> &normales = triangleMesh.getNormales();
		int maxIndexTriangle = triangles.size();

		if (maxIndexTriangle < 8)							// Si le nombre de triangle est inférieur à 8
		{													//On stocke les triangles dans les feuilles
			_triangleMesh = triangleMesh;
			_box = box;
			_lchild = nullptr;
			_rchild = nullptr;
		}
		else
		{
			if (box.isEmpty())
			{
				_triangleMesh = triangleMesh;
				findBoundingBox();
			}
			else
			{
				_box = box;
			}

			std::map<int, std::vector<float>>
				lBox; // mapping left-boxes area with the axis

			std::map<int, std::vector<float>>
				rBox; // mapping right-boxes area with the axis
			Box box, box2;

			MeshTriangle triangle;
			for (int j = 0; j < 3; j++)
			{
				sortTriangles(triangles, vertices,
							  j); // sort en fonction de l'axe 0=x, 1=y, 2=z

				lBox.insert(std::pair<int, std::vector<float>>(
					j, std::vector<float>()));

				rBox.insert(std::pair<int, std::vector<float>>(
					j, std::vector<float>()));

				box.emptyBox();
				box2.emptyBox();

				for (int i = 0; i < maxIndexTriangle; i++)			//Pour chaque axe trié, on va parcourir la liste de triangle dans les deux sens
																	//puis l'on va stocker les boites englobants ceux-ci.
				{													//A chaque itération, on stocke une boite contenant un triangle 
					triangle = triangles[i];						//de plus que la précédente
					int v0 = triangle._v0;
					int v1 = triangle._v1;
					int v2 = triangle._v2;
					box.enlarge(vertices[v0]);
					box.enlarge(vertices[v1]);
					box.enlarge(vertices[v2]);
					lBox[j].push_back(box.getArea());

					triangle = triangles[maxIndexTriangle - 1 - i];
					box2.enlarge(vertices[triangle._v0]);
					box2.enlarge(vertices[triangle._v1]);
					box2.enlarge(vertices[triangle._v2]);
					rBox[j].push_back(box2.getArea());
				}
			}

			int bestAxis = 0, min_id = 0;
			float aireBoundingBox = _box.getArea();
			float SAH = 0;

			float min = FLT_MAX;

			for (int j = 0; j < 3; j++)									//On calcule le SAH le plus optimal  sur les troix axes   
			{															//en parcourant les boites que l'on a stocké précedemment.
				int nbrTRG = 0, nbrTRD = maxIndexTriangle;
				float aireG = 0, aireD = 0;
				for (int i = 0; i < maxIndexTriangle; i++)
				{
					nbrTRG++;
					nbrTRD--;
					aireG = lBox[j][i];
					aireD = rBox[j][maxIndexTriangle - i - 1];
				

					SAH = ((aireG * nbrTRG + aireD * nbrTRD) / aireBoundingBox);

					if (SAH < min)
					{
						min = SAH;

						bestAxis = j;		//L'axe du meilleur SAH calculé
						min_id = i;			//Le nombre de triangle à gauche
					}
				}
			}

			sortTriangles(triangles, vertices, bestAxis);


			std::vector<MeshTriangle> split_left;					//Recupération des triangles gauches et droites
			std::vector<MeshTriangle> split_right;					//sur le meilleur axe

			for (int i = 0; i < maxIndexTriangle; i++)
			{
				if (i <= min_id)
					split_left.push_back(triangles[i]);
				else
					split_right.push_back(triangles[i]);
			}

			Box finalLeftBox;
			Box finalRightBox;

			for (int l = 0; l < split_left.size(); l++)				//On crée les boites englobants les triangles précedents
			{
				finalLeftBox.enlarge(vertices[split_left[l]._v0]);
				finalLeftBox.enlarge(vertices[split_left[l]._v1]);
				finalLeftBox.enlarge(vertices[split_left[l]._v2]);
			}

			for (int l = 0; l < split_right.size(); l++)
			{
				finalRightBox.enlarge(vertices[split_right[l]._v0]);
				finalRightBox.enlarge(vertices[split_right[l]._v1]);
				finalRightBox.enlarge(vertices[split_right[l]._v2]);
			}

			TriangleMesh tl(split_left, vertices, normales);
			TriangleMesh tr(split_right, vertices, normales);

			_lchild = new BVHNode(tl, finalLeftBox);				//On crée des nouveaux fils et continue la création de l'arbre

			_rchild = new BVHNode(tr, finalRightBox);

			
		}
	}


	/*
	 * Fonction permettant de calculer la boite englobant  
	 * les triangles du noeud actuel
	 */

	void BVHNode::findBoundingBox()
	{
		std::vector<MeshTriangle> triangles = _triangleMesh.getTriangles();
		std::vector<Vec3f> vertices = _triangleMesh.getVertices();
		int maxIndexTriangle = triangles.size();

		MeshTriangle triangle;
		uint v0i, v1i, v2i;
		Vec3f v0, v1, v2;

		for (int i = 0; i < maxIndexTriangle; ++i)
		{
			triangle = triangles[i];

			v0i = triangles[i]._v0;
			v1i = triangles[i]._v1;
			v2i = triangles[i]._v2;

			v0 = vertices[v0i];
			v1 = vertices[v1i];
			v2 = vertices[v2i];

			_box.enlarge(v0);
			_box.enlarge(v1);
			_box.enlarge(v2);
		}
	}


	/*
	 * Fonction de tri de la liste de triangle en fonction
	 * des barycentres de ceux-ci
	 */

	void BVHNode::sortTriangles(std::vector<MeshTriangle> &triangles,
								const std::vector<Vec3f> &vertices, int axis)
	{
		int maxIndexTriangle = triangles.size();

		std::sort(triangles.begin(), triangles.end(),
				  [&vertices, &axis, this](const MeshTriangle &a,
										   const MeshTriangle &b) {
					  Vec3f gA = barycentreTriangle(a, vertices);
					  Vec3f gB = barycentreTriangle(b, vertices);
					  return gA[axis] < gB[axis];
				  });
	}

	

	/*
	 * Fonction de récupération du barycentre d'un triangle
	 */

	const Vec3f& BVHNode::barycentreTriangle(const MeshTriangle &m,
									  const std::vector<Vec3f> &vertices)
	{
		Vec3f p0 = vertices[m._v0];
		Vec3f p1 = vertices[m._v1];
		Vec3f p2 = vertices[m._v2];

		float gX = (p0.x + p1.x + p2.x) / 3;
		float gY = (p0.y + p1.y + p2.y) / 3;
		float gZ = (p0.z + p1.z + p2.z) / 3;

		return Vec3f(gX, gY, gZ);
	}

	/*
	 * Fonction permettant de calculer les triangles intersectés entre  
	 * le rayon et le mesh
	 * Elle utilise la fonction getIntersectedTriangles() qui parcourt
	 * le BVH et récupère les triangles des noeuds intersectés
	 */

	TriangleMesh BVHNode::intersect(const Ray &ray, BVHNode *root)
	{
		std::vector<Vec3f> vertices = _triangleMesh.getVertices();
		std::vector<Vec3f> normales = _triangleMesh.getNormales();

		std::vector<MeshTriangle> triangles
			= getIntersectedTriangles(ray, root);

		TriangleMesh tm(triangles, vertices, normales);

		tm.setMaterial(this->_triangleMesh.getMaterial());
		tm.setColorMaterial(this->_triangleMesh.getColorMaterial());
		tm.setReflectionAmount(this->_triangleMesh.getReflectionAmount());
		tm.setRefractionAmount(this->_triangleMesh.getRefractionAmount());
		tm.setRefractionIndex(this->_triangleMesh.getRefractionIndex());
		tm.setRugosity(this->_triangleMesh.getRugosity());
		tm.setf0(this->_triangleMesh.getf0());

		return tm;
	}

	/*
	 * Fonction  qui parcourt le BVH et récupère les triangles des noeuds intersectés
	 */

	std::vector<MeshTriangle> BVHNode::getIntersectedTriangles(const Ray &ray,
															   BVHNode *node)
	{
		std::vector<MeshTriangle> triangles;

		if (node->_box.intersect(ray))
		{
			if (node->_lchild != nullptr)
			{
				std::vector<MeshTriangle> lTriangles
					= getIntersectedTriangles(ray, node->_lchild);
				std::vector<MeshTriangle> rTriangles
					= getIntersectedTriangles(ray, node->_rchild);

				lTriangles.insert(lTriangles.end(), rTriangles.begin(),
								  rTriangles.end());

				triangles.insert(triangles.end(), lTriangles.begin(),
								 lTriangles.end());
			}
			else
			{
				return node->_triangleMesh.getTriangles();
			}
		}

		return triangles;
	}

	BVHNode::~BVHNode()
	{
		delete _lchild;
		delete _rchild;
	}

} // namespace ISICG_ISIR