//GJK�֘A�̃w�b�_�[
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
private://�萔��`
	static const UInt32 MaxVerts = 128;
	static const UInt32 MaxEdges = 128;
	static const UInt32 MaxFacets = 64;

private://�\���̒�`
	//�ʏ��
	struct Facet
	{
		D3DXVECTOR3 normal;		//�@��
		D3DXVECTOR3 closest;	//���_����̍ŋߐړ_
		UInt32 obsolete;		//�p��
		float distSqr;			//�ŋߐڋ�����2��
		int j[3];				//�אږʂ��猩���C���f�b�N�X
		Facet* adj[3];			//�אږ�
		Int8 v[3];				//�ʂ��\�����钸�_
	};

	//�G�b�W���
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

private://�����o�ϐ�

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

public://�����o�֐�
	GjkSolver();
	~GjkSolver(){};
	void Setup(void* sA, void* sB, GetSupportVertexFunc fA, GetSupportVertexFunc fB);

	int Collide(float& distance, D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
		const D3DXMATRIX& transformA, const D3DXMATRIX& transformB, float distanceThreshold = FLT_MAX);

	int Closest(float& distance, D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
		const D3DXMATRIX& transformA, const D3DXMATRIX& transformB, float distanceThreshold = FLT_MAX);


private://�����o�֐�

	//�ʂ�ǉ�
	Facet* AddFacet(int v1, int v2, int v3);

	//�ʂ������N������
	void LinkFacets(Facet* f1, int e1, Facet* f2, int e2)
	{
		f1->adj[e1] = f2;
		f2->adj[e2] = f1;
		f1->j[e1] = e2;
		f2->j[e2] = e1;

	}

	//�V���G�b�g
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

	//�N���[�x�̌��o
	int DetectPenetrationDepth(float& distance, const D3DXMATRIX& transformAB, const D3DXMATRIX& transformBA,
		D3DXVECTOR3& pA, D3DXVECTOR3& pB, D3DXVECTOR3& normal);



};

//�~���R�t�X�L�A���S���Y���N���X
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