//GJK関連のヘッダー
#ifndef _GJK_Solver_h_
#define _GJK_Solver_h_


#include"../Base/Common.h"
#include"SimplexSolver.h"

#define GJK_EPSILON			1e-04f
#define GJK_MARGIN			0.025f
#define GJK_ITERATION_MAX	10
#define EPA_ITERATION_MAX	10

typedef void(*GetSupportVertexFunc)(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex);
typedef void(*GetCenterFunc)(void* shape, D3DXVECTOR3& center);

//gjk

enum eGjkResult
{
	GjkOk=0,
	GjkIntersect,
	GjkInvalid,
};

class SimplexSolver;

class GjkSolver
{
private://定数定義
	static const UInt32 MaxVerts = 128;
	static const UInt32 MaxEdges = 128;
	static const UInt32 MaxFacets = 64;

private://構造体定義
	//面情報
	struct Facet
	{
		D3DXVECTOR3 normal;		//法線
		D3DXVECTOR3 closest;	//原点からの最近接点
		UInt32 obsolete;		//廃棄
		float distSqr;			//最近接距離の2乗
		int j[3];				//隣接面から見たインデックス
		Facet* adj[3];			//隣接面
		Int8 v[3];				//面を構成する頂点
	};

	//エッジ情報
	struct Edge
	{
		Facet* f;
		int i;
		Edge()
		{
			f = nullptr;
			i = 0;
		};

		Edge(Facet* _f, int _i)
		{
			f = _f;
			i = _i;
		}
	};

private://メンバ変数

	SimplexSolver simplex;

	D3DXVECTOR3 vertsP[MaxVerts];
	D3DXVECTOR3 vertsQ[MaxVerts];
	D3DXVECTOR3 vertsW[MaxVerts];

	Facet facets[MaxFacets];
	Facet* facetsHead[MaxFacets];
	Edge edges[MaxEdges];

	D3DXVECTOR3* pVertsP;
	D3DXVECTOR3* pVertsQ;
	D3DXVECTOR3* pVertsW;

	Facet* pFacets;
	Facet** ppFacetsHead;
	Edge* pEdges;

	int numVerts;
	int numEdges;
	int numFacets;
	int numFacetsHead;

	void* shapeA;
	void* shapeB;

	GetSupportVertexFunc getSupportVertexShapeA;
	GetSupportVertexFunc getSupportVertexShapeB;

public://メンバ関数
	GjkSolver();
	~GjkSolver(){};
	void Setup(void* sA, void* sB, GetSupportVertexFunc fA, GetSupportVertexFunc fB);

	int Collide(float& distance, D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
		const D3DXMATRIX& transformA, const D3DXMATRIX& transformB, float distanceThreshold = FLT_MAX);

	int Closest(float& distance, D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
		const D3DXMATRIX& transformA, const D3DXMATRIX& transformB, float distanceThreshold = FLT_MAX);


private://メンバ関数

	//面を追加
	Facet* AddFacet(int v1, int v2, int v3);

	//面をリンクさせる
	void LinkFacets(Facet* f1, int e1, Facet* f2, int e2)
	{
		f1->adj[e1] = f2;
		f2->adj[e2] = f1;
		f1->j[e1] = e2;
		f2->j[e2] = e1;

	}

	//シルエット
	bool Silhouette(Facet* facet, int i, D3DXVECTOR3& w);


	bool OriginInTetrahedron(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3)
	{
		D3DXVECTOR3 n0 = GameMath::Cross((p1 - p0), (p2 - p0));
		D3DXVECTOR3 n1 = GameMath::Cross((p2 - p1), (p3 - p1));
		D3DXVECTOR3 n2 = GameMath::Cross((p3 - p2), (p0 - p2));
		D3DXVECTOR3 n3 = GameMath::Cross((p0 - p3), (p1 - p3));

		return D3DXVec3Dot(&n0, &p0)*D3DXVec3Dot(&n0, &(p3 - p0)) <= 0.0f&&
			D3DXVec3Dot(&n1, &p1)*D3DXVec3Dot(&n1, &(p0 - p1)) <= 0.0f&&
			D3DXVec3Dot(&n2, &p2)*D3DXVec3Dot(&n2, &(p1 - p2)) <= 0.0f&&
			D3DXVec3Dot(&n3, &p3)*D3DXVec3Dot(&n3, &(p2 - p3)) <= 0.0f;

	}

	//侵入深度の検出
	int DetectPenetrationDepth(float& distance, const D3DXMATRIX& transformAB, const D3DXMATRIX& transformBA,
		D3DXVECTOR3& pA, D3DXVECTOR3& pB, D3DXVECTOR3& normal);



};

//ミンコフスキアルゴリズムクラス
class MinkowskiSolver
{
private:
	void* shapeA;
	void* shapeB;
	GetSupportVertexFunc getSupportVertexShapeA;
	GetSupportVertexFunc getSupportVertexShapeB;
	GetCenterFunc getCenterA;
	GetCenterFunc getCenterB;

public:

	void Setup(void* sA, void* sB, GetSupportVertexFunc fsA, GetSupportVertexFunc fsB, GetCenterFunc fcA, GetCenterFunc fcB);

	float Collide(D3DXVECTOR3& normal,D3DXVECTOR3& pointA,D3DXVECTOR3& pointB,
		const D3DXMATRIX& transformA, const D3DXMATRIX& transformB, float distanceThreshold = FLT_MAX);
};


#endif