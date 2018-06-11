#include"GJK_Solver.h"
#include"../../Utility/Macro.h"



template <class T,int maxData=20>
struct GjkStack
{
	T data[maxData];
	int cur;
	bool isOk;


	GjkStack() :isOk(true), cur(0){};

	void Push(const T& d)
	{
		if (cur < maxData)
		{
			data[cur] = d;
			++cur;
		}
		else
		{
			isOk = false;
		}
	}

	T Pop()
	{
		--cur;
		return data[cur];
	}

	bool isEmpty()
	{
		return cur == 0;
	}

};


GjkSolver::GjkSolver() :shapeA(nullptr), shapeB(nullptr)
{
	pVertsP = vertsP;
	pVertsQ = vertsQ;
	pVertsW = vertsW;
	pFacets = facets;
	ppFacetsHead = facetsHead;
	pEdges = edges;
}

void GjkSolver::Setup(void* sA, void* sB, GetSupportVertexFunc fA, GetSupportVertexFunc fB)
{
	shapeA = sA;
	shapeB = sB;
	getSupportVertexShapeA = fA;
	getSupportVertexShapeB = fB;
}

GjkSolver::Facet* GjkSolver::AddFacet(int v1, int v2, int v3)
{
	if (numFacets >= MaxFacets)
		return nullptr;

	Facet& facet = pFacets[numFacets];

	D3DXVECTOR3 ver1 = vertsW[v1];
	D3DXVECTOR3 ver2 = vertsW[v2];
	D3DXVECTOR3 ver3 = vertsW[v3];
	facet.obsolete = 0;
	facet.v[0] = v1;
	facet.v[1] = v2;
	facet.v[2] = v3;


	D3DXVECTOR3 normal = GameMath::Cross((ver3 - ver1), (ver2 - ver1));

	float l = D3DXVec3LengthSq(&normal);

	if (l < GJK_EPSILON*GJK_EPSILON)
		return nullptr;

	normal /= sqrtf(l);

	float closestLength = D3DXVec3Dot(&ver1, &normal);
	facet.distSqr = closestLength*closestLength;
	facet.closest = closestLength*normal;
	facet.normal = normal;

	ppFacetsHead[numFacetsHead] = &facet;
	++numFacetsHead;
	++numFacets;

	return &facet;

}

bool GjkSolver::Silhouette(Facet* facet, int i, D3DXVECTOR3& w)
{
	GjkStack<Edge> gs;
	gs.Push(Edge(facet, i));


	do{

		Edge stk = gs.Pop();
		Facet* ft = stk.f;

		if (ft->obsolete == 0)
		{
			//wから見えるか判定
			if (D3DXVec3Dot(&ft->normal, &(w - ft->closest)) < 0.0f)
			{
				//見えないからエッジ登録
				if ((unsigned int)numEdges >= MaxEdges)
					return false;
				pEdges[numEdges] = stk;
				numEdges++;

			}
			else
			{
				ft->obsolete = 1;
				gs.Push(Edge(ft->adj[(stk.i + 2) % 3], ft->j[(stk.i + 2) % 3]));
				gs.Push(Edge(ft->adj[(stk.i + 1) % 3], ft->j[(stk.i + 1) % 3]));

			}

		}
		

	} while (!gs.isEmpty());

	return gs.isOk;
}


int GjkSolver::DetectPenetrationDepth(
	float& distance,
	const D3DXMATRIX& transformAB, const D3DXMATRIX& transformBA,
	D3DXVECTOR3& pA, D3DXVECTOR3& pB, D3DXVECTOR3& normal)
{

	D3DXMATRIX matrixAB, matrixBA;
	D3DXVECTOR3 offsetAB, offsetBA;

	matrixAB = GameMath::Get3x3(transformAB);
	offsetAB = GameMath::GetTranslation(transformAB);

	matrixBA = GameMath::Get3x3(transformBA);
	offsetBA = GameMath::GetTranslation(transformBA);

	int epaIterationCount = 0;


	numFacets = 0;
	numFacetsHead = 0;

 	if (simplex.numVertices <= 1)
	{
		return eGjkResult::GjkInvalid;

	}
	else if (simplex.numVertices == 2)
	{
		D3DXVECTOR3 v0 = simplex.w[0];
		D3DXVECTOR3 v1 = simplex.w[1];
		D3DXVECTOR3 dir = GameMath::Normalize(v1 - v0);
		D3DXMATRIX rot = GameMath::Rotation3x3(D3DXToRadian(120.0f), dir);

		D3DXVECTOR3 vec(0.0f, 0.0f, 0.0f);
		if (dir.x < dir.y)
		{
			if (dir.x < dir.z)
			{
				vec.x = 1.0f;
			}
			else
			{
				vec.z = 1.0f;
			}
		}
		else
		{
			if (dir.y < dir.z)
			{
				vec.y = 1.0f;
			}
			else
			{
				vec.z = 1.0f;
			}
		}

		D3DXVECTOR3 aux[3];
		aux[0] = GameMath::Cross(dir, vec);			//0deg
		aux[1] = GameMath::Rotate(aux[0], rot);		//120deg
		aux[2] = GameMath::Rotate(aux[1], rot);		//240deg

		D3DXVECTOR3 p[3], q[3], w[3];
		
		for (int i = 0; i < 3; ++i)
		{
			D3DXVECTOR3 pInA, qInB;
			getSupportVertexShapeA(shapeA, aux[i], pInA);
			getSupportVertexShapeB(shapeB, GameMath::Rotate(-aux[i], matrixBA), qInB);
			p[i] = pInA;
			q[i] = offsetAB + GameMath::Rotate(qInB, matrixAB);
			w[i] = p[i] - q[i];
			pVertsP[i] = p[i];
			pVertsQ[i] = q[i];
			pVertsW[i] = w[i];

		}

		if (OriginInTetrahedron(w[0], w[1], w[2], v0))
		{
			vertsP[3] = simplex.p[0];
			vertsQ[3] = simplex.q[0];
			vertsW[3] = simplex.w[0];
			numVerts = 4;
		}
		else if (OriginInTetrahedron(w[0], w[1], w[2], v1))
		{
			vertsP[3] = simplex.p[1];
			vertsQ[3] = simplex.q[1];
			vertsW[3] = simplex.w[1];
			numVerts = 4;
		}
		else
		{
			return eGjkResult::GjkInvalid;
		}
	}
	else if (simplex.numVertices==3)
	{
		numVerts = 3;
		for(int i = 0; i < 3; ++i)
		{
			vertsP[i] = simplex.p[i];
			vertsQ[i] = simplex.q[i];
			vertsW[i] = simplex.w[i];
		}

		D3DXVECTOR3 p[2], q[2], w[2];
		{
			D3DXVECTOR3 v = GameMath::Cross(pVertsW[2] - pVertsW[0], pVertsW[1] - pVertsW[0]);
			D3DXVECTOR3 pInA, qInB;
			getSupportVertexShapeA(shapeA, v, pInA);
			getSupportVertexShapeB(shapeB, GameMath::Rotate(-v, matrixBA), qInB);

			p[0] = pInA;
			q[0] = offsetAB + GameMath::Rotate(qInB, matrixAB);
			w[0] = p[0] - q[0];

			getSupportVertexShapeA(shapeA, (-v), pInA);
			getSupportVertexShapeB(shapeB, GameMath::Rotate(v, matrixBA), qInB);

			p[1] = pInA;
			q[1] = offsetAB + GameMath::Rotate(qInB, matrixAB);
			w[1] = p[1] - q[1];
		}

		if (OriginInTetrahedron(vertsW[0], vertsW[1], vertsW[2], w[0]))
		{
			pVertsP[3] = p[0];
			pVertsQ[3] = q[0];
			pVertsW[3] = w[0];
			numVerts = 4;
		}
		else if (OriginInTetrahedron(vertsW[0], vertsW[1], vertsW[2], w[1]))
		{
			pVertsP[3] = p[1];
			pVertsQ[3] = q[1];
			pVertsW[3] = w[1];
			numVerts = 4;
		}
		else
		{
			return eGjkResult::GjkInvalid;
		}

	}
	else
	{
		numVerts = 4;
		for (int i = 0; i < numVerts; ++i)
		{
			pVertsP[i] = simplex.p[i];
			pVertsQ[i] = simplex.q[i];
			pVertsW[i] = simplex.w[i];
		}
	}


	if (numVerts != 4)
	{
		return eGjkResult::GjkInvalid;
	}

	//原点が内部にあるか判定
	if (!OriginInTetrahedron(vertsW[0], vertsW[1], vertsW[2], vertsW[3]))
	{
		return eGjkResult::GjkInvalid;
	}


	//面の向きチェック
	if (D3DXVec3Dot(&(-pVertsW[0]),&GameMath::Cross(vertsW[2] - vertsW[0], vertsW[1] - vertsW[0])) > 0.0f)
	{
		//面を反転
		D3DXVECTOR3 p1, q1, w1;
		
		p1 = pVertsP[1];
		q1 = pVertsQ[1];
		w1 = pVertsW[1];

		pVertsP[1] = pVertsP[3];
		pVertsQ[1] = pVertsQ[3];
		pVertsW[1] = pVertsW[3];

		pVertsP[3] = p1;
		pVertsQ[3] = q1;
		pVertsW[3] = w1;

	}

	{
		Facet* f0 = AddFacet(0, 1, 2);
		Facet* f1 = AddFacet(0, 3, 1);
		Facet* f2 = AddFacet(0, 2, 3);
		Facet* f3 = AddFacet(1, 3, 2);

		if (f0 == nullptr || f1 == nullptr || f2 == nullptr || f3 == nullptr)
		{
			return eGjkResult::GjkInvalid;
		}

		LinkFacets(f0, 0, f1, 2);
		LinkFacets(f0, 1, f3, 2);
		LinkFacets(f0, 2, f2, 0);
		LinkFacets(f1, 0, f2, 2);
		LinkFacets(f1, 1, f3, 0);
		LinkFacets(f2, 1, f3, 1);

	}

	//探索
	Facet* facetMin = nullptr;

	do
	{

		//原点から一番近い点を調べてそのベクトルと支点を返す
		int minFacetIdx = 0;
		{
			float minDistSqr = ppFacetsHead[0]->distSqr;
			facetMin = ppFacetsHead[0];
			for (int i = 1; i < numFacetsHead; ++i)
			{
				if (ppFacetsHead[i]->obsolete == 0 && ppFacetsHead[i]->distSqr < minDistSqr)
				{
					minDistSqr = ppFacetsHead[i]->distSqr;
					facetMin = ppFacetsHead[i];
					minFacetIdx = i;
				}
			}

		}

		//リストから外す
		--numFacetsHead;
		ppFacetsHead[minFacetIdx] = ppFacetsHead[numFacetsHead];

		D3DXVECTOR3 pInA(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 qInB(0.0f, 0.0f, 0.0f);
		getSupportVertexShapeA(shapeA, facetMin->normal, pInA);
		getSupportVertexShapeB(shapeB, GameMath::Rotate(-facetMin->normal, matrixBA), qInB);
		D3DXVECTOR3 q = offsetAB + GameMath::Rotate(qInB, matrixAB);
		D3DXVECTOR3 w = pInA - q;
		D3DXVECTOR3 v = facetMin->closest;

		//最近接点チェック
		float l0 = D3DXVec3Length(&v);
		float l1 = D3DXVec3Dot(&facetMin->normal, &w);

		if (l1 - l0 < GJK_EPSILON)
			break;

		//求めた点を追加して面分割
		{

			if (numVerts >= MaxVerts)
				break;


			int vID = numVerts;
			numVerts++;

			pVertsP[vID] = pInA;
			pVertsQ[vID] = q;
			pVertsW[vID] = w;

			facetMin->obsolete = 1;

			numEdges = 0;

			if (!Silhouette(facetMin->adj[0], facetMin->j[0], w) ||
				!Silhouette(facetMin->adj[1], facetMin->j[1], w) ||
				!Silhouette(facetMin->adj[2], facetMin->j[2], w))
			{
				break;
			}

			if (numEdges == 0)
				break;

			bool edgeCheck = true;

			Facet* firstFacet;
			Facet* lastFacet;
			{
				Edge& edge = pEdges[0];
				
				int v0 = edge.f->v[(edge.i + 1) % 3];
				int v1 = edge.f->v[edge.i];

				firstFacet = AddFacet(v0, v1, vID);

				if (!firstFacet)
				{
					edgeCheck = false;
					break;
				}

				LinkFacets(edge.f, edge.i, firstFacet, 0);
				lastFacet = firstFacet;
			}

			for (int i = 1; i < numEdges; ++i)
			{
				Edge& edge = pEdges[i];
				int v0 = edge.f->v[(edge.i + 1) % 3];
				int v1 = edge.f->v[edge.i];

				Facet* f = AddFacet(v0, v1, vID);
				if (!f)
				{
					edgeCheck = false;
					break;
				}

				LinkFacets(edge.f, edge.i, f, 0);
				LinkFacets(f, 2, lastFacet, 1);
				lastFacet = f;
			}
			if (!edgeCheck)
				break;

			LinkFacets(lastFacet, 1, firstFacet, 2);


		}
		++epaIterationCount;

		if (epaIterationCount > EPA_ITERATION_MAX || numFacetsHead == 0)
			break;

	} while (1);

	if (!facetMin)
	{
		return eGjkResult::GjkInvalid;
	}

	//衝突点計算
	int v1 = facetMin->v[0];
	int v2 = facetMin->v[1];
	int v3 = facetMin->v[2];


	D3DXVECTOR3 tmp0 = pVertsW[v2] - pVertsW[v1];
	D3DXVECTOR3 tmp1 = pVertsW[v3] - pVertsW[v1];
	D3DXVECTOR3 tmp2 = facetMin->closest - pVertsW[v1];


	float dot00 = D3DXVec3Dot(&tmp0, &tmp0);
	float dot01 = D3DXVec3Dot(&tmp0, &tmp1);
	float dot11 = D3DXVec3Dot(&tmp1, &tmp1);
	float dot20 = D3DXVec3Dot(&tmp2, &tmp0);
	float dot21 = D3DXVec3Dot(&tmp2, &tmp1);
	float denom = dot00*dot11 - dot01*dot01;

	if (denom*denom < GJK_EPSILON*GJK_EPSILON)
		return eGjkResult::GjkInvalid;

	float v = (dot11*dot20 - dot01*dot21) / denom;
	float w = (dot00*dot21 - dot01*dot20) / denom;

	pA = pVertsP[v1] + v*(pVertsP[v2] - pVertsP[v1]) + w*(pVertsP[v3] - pVertsP[v1]);
	pB = pVertsQ[v1] + v*(pVertsQ[v2] - pVertsQ[v1]) + w*(pVertsQ[v3] - pVertsQ[v1]);

	float lenSqr = D3DXVec3LengthSq(&(pB - pA));
	if (lenSqr < GJK_EPSILON*GJK_EPSILON)
	{
		normal = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		distance = 0.0f;
	}
	else
	{
		float len = sqrtf(lenSqr);
		normal = (pB - pA) / len;
		distance = -len;
	}

	return eGjkResult::GjkOk;

}


//衝突判定
int GjkSolver::Collide(float& distance, D3DXVECTOR3& normal, 
	D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	const D3DXMATRIX& transformA, const D3DXMATRIX& transformB, 
	float distanceThreshold)
{

	int gjkIterationCount = 0;

	simplex.Reset();

	D3DXMATRIX transformAB, transformBA;
	D3DXMATRIX matrixAB, matrixBA;

	D3DXVECTOR3 offsetAB, offsetBA;

	transformAB = transformB* GameMath::OrthoInverse3x4(transformA);
	matrixAB = GameMath::Get3x3(transformAB);
	offsetAB = GameMath::GetTranslation(transformAB);


	transformBA = transformA* GameMath::OrthoInverse3x4(transformB);
	matrixBA = GameMath::Get3x3(transformBA);
	offsetBA = GameMath::GetTranslation(transformBA);

	D3DXVECTOR3 separatingAxis = -offsetBA;

	//0ベクトルか判定
	if (D3DXVec3LengthSq(&separatingAxis) < 0.00001f)
	{
		separatingAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	}

	float squaredDistance = FLT_MAX;

	float delta = 0.0f;

	float _distance = FLT_MAX;
	
	while (1)
	{
		D3DXVECTOR3 pInA, qInB;

		getSupportVertexShapeA(shapeA, (-separatingAxis), pInA);
		getSupportVertexShapeB(shapeB, GameMath::Rotate(separatingAxis, matrixBA), qInB);

		D3DXVECTOR3 q = offsetAB + GameMath::Rotate(qInB, matrixAB);
		D3DXVECTOR3 w = pInA - q;

		delta = D3DXVec3Dot(&separatingAxis, &w);

		//早期終了チェック
		if (delta > 0.0f)
		{
			normal = separatingAxis;
			distance = _distance;
			return eGjkResult::GjkOk;
		}

		//同一頂点が存在しないか確認
		if (simplex.InSimplex(w))
			break;

		float f0 = squaredDistance - delta;
		float f1 = squaredDistance * GJK_EPSILON;

		if (f0 <= f1)
			break;


		//頂点追加
		simplex.AddVertex(w, pInA, q);

		//原点と単体の最近接点を求めて分離軸を返す

		if (!simplex.Closest(separatingAxis))
		{
			normal = separatingAxis;
			distance = _distance;
			return eGjkResult::GjkInvalid;
		}

		squaredDistance = D3DXVec3LengthSq(&separatingAxis);

		if (gjkIterationCount >= GJK_ITERATION_MAX || simplex.fullSimplex())
			break;


		++gjkIterationCount;

	}

	D3DXVECTOR3 pA(0.0f, 0.0f, 0.0f), pB(0.0f, 0.0f, 0.0f), nA(0.0f, 0.0f, 0.0f);

	//convexは交差しているから接触点を探す
	int ret = DetectPenetrationDepth(_distance, transformAB, transformBA, pA, pB, nA);
	
	if (ret != eGjkResult::GjkOk)
		return ret;

	distance = _distance;

	//マージン考慮
	if (_distance < 0.0f)
	{
		pA += nA*GJK_MARGIN;
		pB -= nA*GJK_MARGIN;
		distance = D3DXVec3Dot(&nA, &(pA - pB));
		normal = nA;
		pointA = pA;
		pointB = pB;
	}
	return eGjkResult::GjkOk;


}

int GjkSolver::Closest(float& distance, D3DXVECTOR3& normal,
	D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	const D3DXMATRIX& transformA, const D3DXMATRIX& transformB,
	float distanceThreshold)
{

	int gjkIterationCount = 0;
	simplex.Reset();

	D3DXMATRIX transformAB, transformBA;
	D3DXMATRIX matrixAB, matrixBA;
	D3DXVECTOR3 offsetAB, offsetBA;

	transformAB = transformB* GameMath::OrthoInverse3x4(transformA);
	matrixAB = GameMath::Get3x3(transformAB);
	offsetAB = GameMath::GetTranslation(transformAB);

	transformBA = transformA* GameMath::OrthoInverse3x4(transformB);
	matrixBA = GameMath::Get3x3(transformBA);
	offsetBA = GameMath::GetTranslation(transformBA);

	D3DXVECTOR3 separatingAxis = -offsetAB;
	if (D3DXVec3LengthSq(&separatingAxis) < 0.000001f)
		separatingAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

	float squaredDistance = FLT_MAX;
	float delta = 0.0f;
	float _distance = FLT_MAX;

	while (1)
	{
		D3DXVECTOR3 pInA, qInB;

		getSupportVertexShapeA(shapeA, (-separatingAxis), pInA);
		getSupportVertexShapeB(shapeB, GameMath::Rotate(separatingAxis, matrixBA), qInB);

		D3DXVECTOR3 q = offsetAB + GameMath::Rotate(qInB, matrixAB);
		D3DXVECTOR3 w = pInA - q;

		delta = D3DXVec3Dot(&separatingAxis, &w);

		//同一頂点が存在しないか確認
		if (simplex.InSimplex(w))
			break;

		float f0 = squaredDistance - delta;
		float f1 = squaredDistance * GJK_EPSILON;

		if (f0 <= f1)
			break;

		simplex.AddVertex(w, pInA, q);

		//原点との最近接点を求めて分離軸を返す
		if (!simplex.Closest(separatingAxis))
			return eGjkResult::GjkInvalid;

		//フルシンプレックスの時交差している（原点は内部）
		if (simplex.fullSimplex())
			return eGjkResult::GjkIntersect;

		squaredDistance = D3DXVec3LengthSq(&separatingAxis);

		//反復回数が上限に達したから現在の値を近似値として返す
		if (gjkIterationCount >= GJK_ITERATION_MAX)
			break;

		++gjkIterationCount;

	}

	D3DXVECTOR3 pA(0.0f, 0.0f, 0.0f), pB(0.0f, 0.0f, 0.0f), nA(0.0f, 0.0f, 0.0f);
	simplex.GetClosestPoints(pA, pB);

	if (squaredDistance < GJK_EPSILON* GJK_EPSILON)
		nA = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	else
	nA = GameMath::Normalize(separatingAxis);

	distance = D3DXVec3Dot(&nA, &(pA - pB));
	normal = nA;
	pointA = pA;
	pointB = pB;

	return eGjkResult::GjkOk;


}


//ここからミンコフスキ
#define MINKOWSKI_EPSILON 0.00001f
#define MINKOWSKI_MAX_ITERATION 32

void MinkowskiSolver::Setup(void* sA, void* sB, GetSupportVertexFunc fsA, GetSupportVertexFunc fsB, GetCenterFunc fcA, GetCenterFunc fcB)
{
	shapeA = sA;
	shapeB = sB;
	getSupportVertexShapeA = fsA;
	getSupportVertexShapeB = fsB;
	getCenterA = fcA;
	getCenterB = fcB;

}


float MinkowskiSolver::Collide(D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	const D3DXMATRIX& transformA, const D3DXMATRIX& transformB, float distanceThreshold)
{
	D3DXMATRIX transformAB, transformBA;
	D3DXMATRIX matrixAB, matrixBA;
	D3DXVECTOR3 offsetAB, offsetBA;

	transformAB = transformB* GameMath::OrthoInverse3x4(transformA);
	matrixAB = GameMath::Get3x3(transformAB);
	offsetAB = GameMath::GetTranslation(transformAB);

	transformBA = transformA* GameMath::OrthoInverse3x4(transformB);
	matrixBA = GameMath::Get3x3(transformBA);
	offsetBA = GameMath::GetTranslation(transformBA);

	//原点作成
	D3DXVECTOR3 portalOrigin(0.0f, 0.0f, 0.0f);
	{
		D3DXVECTOR3 centerA(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 centerB(0.0f, 0.0f, 0.0f);

		getCenterA(shapeA, centerA);
		getCenterB(shapeB, centerB);

		portalOrigin = 0.5f * (centerA - (offsetAB + GameMath::Rotate(centerB, matrixAB)));


	}

	D3DXVECTOR3 portal1, portal2, portal3;
	D3DXVECTOR3 p1, p2, p3, q1, q2, q3;
	D3DXVECTOR3 s, pInA, qInB, p, q;

	//初期ポータル作成
	{

		s = -portalOrigin;
		getSupportVertexShapeA(shapeA, s, pInA);
		getSupportVertexShapeB(shapeB, GameMath::Rotate(-s, matrixBA), qInB);

		p = pInA;
		q = offsetAB + GameMath::Rotate(qInB, matrixAB);

		p1 = p;
		q1 = qInB;
		portal1 = p - q;

		s = GameMath::Cross(portal1, portalOrigin);

		getSupportVertexShapeA(shapeA, s, pInA);
		getSupportVertexShapeB(shapeB, GameMath::Rotate(-s, matrixBA), qInB);

		p = pInA;
		q = offsetAB + GameMath::Rotate(qInB, matrixAB);

		p2 = pInA;
		q2 = qInB;
		portal2 = p - q;

		s = GameMath::Cross(portal1 - portalOrigin, portal2 - portalOrigin);
		getSupportVertexShapeA(shapeA, s, pInA);
		getSupportVertexShapeB(shapeB, GameMath::Rotate(-s, matrixBA), qInB);

		p = pInA;
		q = offsetAB + GameMath::Rotate(qInB, matrixAB);
		p3 = pInA;
		q3 = qInB;

		portal3 = p - q;


		//原点がポータルの中にあるか確認
		//なければ作り直し
		bool originIsInsidePortal = false;
		int createCount = 0;
		do{
			D3DXVECTOR3 a = portal1 - portalOrigin;
			D3DXVECTOR3 b = portal2 - portalOrigin;
			D3DXVECTOR3 c = portal3 - portalOrigin;
			D3DXVECTOR3 n1 = GameMath::Cross(a, c);
			D3DXVECTOR3 n2 = GameMath::Cross(b, a);
			D3DXVECTOR3 n3 = GameMath::Cross(c, b);

			float distFacet1 = D3DXVec3Dot(&n1, &-portalOrigin);
			float distFacet2 = D3DXVec3Dot(&n2, &-portalOrigin);
			float distFacet3 = D3DXVec3Dot(&n3, &-portalOrigin);

			if (distFacet1 > 0.0f)
			{
				SWAP(p1, p3);
				SWAP(q1, q3);
				SWAP(portal1, portal3);

				s = n1;
				getSupportVertexShapeA(shapeA, s, pInA);
				getSupportVertexShapeB(shapeB, GameMath::Rotate(-s, matrixBA), qInB);

				p = pInA;
				q = offsetAB + GameMath::Rotate(qInB, matrixAB);
				p2 = pInA;
				q2 = qInB;
				portal2 = p - q;

			}
			else if (distFacet2 > 0.0f)
			{
				SWAP(p1, p2);
				SWAP(q1, q2);
				SWAP(portal1, portal2);

				s = n2;
				getSupportVertexShapeA(shapeA, s, pInA);
				getSupportVertexShapeB(shapeB, GameMath::Rotate(-s, matrixBA), qInB);

				p = pInA;
				q = offsetAB + GameMath::Rotate(qInB, matrixAB);
				p3 = pInA;
				q3 = qInB;
				portal3 = p - q;

			}
			else if (distFacet3 > 0.0f)
			{
				SWAP(p2, p3);
				SWAP(q2, q3);
				SWAP(portal2, portal3);

				s = n3;
				getSupportVertexShapeA(shapeA, s, pInA);
				getSupportVertexShapeB(shapeB, GameMath::Rotate(-s, matrixBA), qInB);

				p = pInA;
				q = offsetAB + GameMath::Rotate(qInB, matrixAB);
				p1 = pInA;
				q1 = qInB;
				portal1 = p - q;
			}
			else
				originIsInsidePortal = true;

			if (createCount > 32)
			{
				return FLT_MAX;
			}
			++createCount;


		} while (!originIsInsidePortal);

	}

	int minkowskiCount = 0;

	do
	{
		//サーフェスに近い新たな点を求める
		s = GameMath::Cross(portal2 - portal1, portal3 - portal1);
		getSupportVertexShapeA(shapeA, s, pInA);
		getSupportVertexShapeB(shapeB, GameMath::Rotate(-s, matrixBA), qInB);
		p = pInA;
		q = offsetAB + GameMath::Rotate(qInB, matrixAB);
		D3DXVECTOR3 newPortal = p - q;

		//原点とサポート平面の位置関係確認
		if (D3DXVec3Dot(&s, &(-newPortal)) > 0.0f)
		{
			return FLT_MAX;
		}

		//ポータルが十分サーフェスに近づいたか確認
		if (D3DXVec3Dot(&s, &(newPortal - portal1)) < MINKOWSKI_EPSILON)
		{
			break;
		}

		if (minkowskiCount > MINKOWSKI_MAX_ITERATION)
			break;
		++minkowskiCount;


		//ポータルリファイン
		D3DXVECTOR3 tmpVec = GameMath::Cross(newPortal, portalOrigin);
		float d1 = D3DXVec3Dot(&(portal1 - portalOrigin), &tmpVec);
		float d2 = D3DXVec3Dot(&(portal2 - portalOrigin), &tmpVec);
		float d3 = D3DXVec3Dot(&(portal3 - portalOrigin), &tmpVec);

		if (d2>0.0f&&d3 <= 0.0f)
		{
			p1 = pInA;
			q1 = qInB;
			portal1 = newPortal;
		}
		else if (d1 > 0.0f&&d2 <= 0.0f)
		{
			p3 = pInA;
			q3 = qInB;
			portal3 = newPortal;
		}
		else if (d3 > 0.0f&&d1 <= 0.0f)
		{
			p2 = pInA;
			q2 = qInB;
			portal2 = newPortal;
		}

	} while (1);


	//ポータルと原点の最近接点が衝突点に
	D3DXMATRIX mtx = GameMath::Get3x3(portal1, portal2, portal3);
	float det = GameMath::Determinant(mtx);

	if (det*det < MINKOWSKI_EPSILON*MINKOWSKI_EPSILON)
		return FLT_MAX;

	D3DXVECTOR3 dir = GameMath::Normalize(s);
	D3DXVECTOR3 closest = D3DXVec3Dot(&portal1, &dir)*dir;
	D3DXVECTOR3 uvw = GameMath::Rotate(closest, GameMath::Inverse3x3(mtx));
	D3DXVECTOR3 pA = GameMath::Rotate(uvw, GameMath::Get3x3(p1, p2, p3));
	D3DXVECTOR3 pB = GameMath::Rotate(uvw, GameMath::Get3x3(q1, q2, q3));

	pA -= dir*GJK_MARGIN;
	pB += dir*GJK_MARGIN;

	D3DXVECTOR3 wA = GameMath::GetTranslation(transformA) +
		GameMath::Rotate(pA, GameMath::Get3x3(transformA));

	D3DXVECTOR3 wB = GameMath::GetTranslation(transformB) +
		GameMath::Rotate(pB, GameMath::Get3x3(transformB));

	D3DXVECTOR3 wN = -dir;
	float dist = D3DXVec3Dot(&wN, &(wA - wB));

	pointA = pA;
	pointB = pB;
	normal = wN;

	return dist;


}