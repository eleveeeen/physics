#include"shape.h"
#include"Contact.h"
#include"GJK_Solver.h"
#include"GJK_SupportFunc.h"
#include"DetectCollisionFunc.h"

//ダミー関数
void DetectCollisionDummy(
	ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{

}

//ボックスとボックスの判定
void DetectCollisionBoxBox(
	ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{
	Box boxA = shapeA.GetBox();
	Box boxB = shapeB.GetBox();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;


	//衝突判定を取って侵入深度をもらう
	float d = ContactBoxBox(normal, pA, pB, &boxA, worldTransformA, &boxB, worldTransformB);

	//衝突していたら追加
	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, -normal, GameMath::Rotate(pA, offsetTransformA), 
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}


}

//ボックスとカプセルの判定
void DetectCollisionBoxCapsule(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{

	Box boxA = shapeA.GetBox();
	Capsule capsuleB = shapeB.GetCapsule();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;

	float d = ContactBoxCapsule(normal, pA, pB, &boxA, worldTransformA, &capsuleB, worldTransformB);

	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, -normal, GameMath::Rotate(pA, offsetTransformA),
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}

}

//ボックスとスフィア
void DetectCollisionBoxSphere(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{
	Box boxA = shapeA.GetBox();
	Sphere sphereB = shapeB.GetSphere();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;

	float d = ContactBoxSphere(normal, pA, pB, &boxA, worldTransformA, &sphereB, worldTransformB);

	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, -normal, GameMath::Rotate(pA, offsetTransformA),
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}

}


//カプセルとボックス
void DetectCollisionCapsuleBox(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{

	Capsule capsuleA = shapeA.GetCapsule();
	Box boxB = shapeB.GetBox();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;

	float d = ContactBoxCapsule(normal, pB, pA, &boxB, worldTransformB, &capsuleA, worldTransformA);

	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, normal, GameMath::Rotate(pA, offsetTransformA),
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}

}

//カプセルとカプセル
void DetectCollisionCapsuleCapsule(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{
	Capsule capsuleA = shapeA.GetCapsule();
	Capsule capsuleB = shapeB.GetCapsule();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;

	float d = ContactCapsuleCapsule(normal, pA, pB, &capsuleA, worldTransformA, &capsuleB, worldTransformB);

	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, -normal, GameMath::Rotate(pA, offsetTransformA),
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}

}

//カプセルとスフィア
void DetectCollisionCapsuleSphere(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{
	Capsule capsuleA = shapeA.GetCapsule();
	Sphere sphereB = shapeB.GetSphere();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;

	float d = ContactCapsuleSphere(normal, pA, pB, &capsuleA, worldTransformA, &sphereB, worldTransformB);

	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, -normal, GameMath::Rotate(pA, offsetTransformA),
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}
}

//スフィアとボックス
void DetectCollisionSphereBox(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{
	Sphere sphereA = shapeA.GetSphere();
	Box boxB = shapeB.GetBox();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;

	float d = ContactBoxSphere(normal, pB, pA, &boxB, worldTransformB, &sphereA, worldTransformA);
	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, normal, GameMath::Rotate(pA, offsetTransformA),
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}
}

//スフィアとカプセル
void DetectCollisionSphereCapsule(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{
	Sphere sphereA = shapeA.GetSphere();
	Capsule capsuleB = shapeB.GetCapsule();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;

	float d = ContactCapsuleSphere(normal, pB, pA, &capsuleB, worldTransformB, &sphereA, worldTransformA);
	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, normal, GameMath::Rotate(pA, offsetTransformA),
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}

}



//スフィアとスフィア
void DetectCollisionSphereSphere(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{
	Sphere sphereA = shapeA.GetSphere();
	Sphere sphereB = shapeB.GetSphere();

	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;

	float d = ContactSphereSphere(normal, pA, pB, &sphereA, worldTransformA, &sphereB, worldTransformB);


	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, -normal, GameMath::Rotate(pA, offsetTransformA),
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}

}

//Gjk
void DetectCollisionGjk(ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{
	int err;
	float d = FLT_MAX;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 pA, pB;
	GjkSolver gjk;

	//shapeAとshapeBの形状に応じていずれかの処理に入る
	//シリンダーが使われていないとこの関数に入らないからどこにもヒットしないことはないはず

	if (shapeA.GetType() == eShapeType::eCylinder)
	{
		Cylinder cylinderA = shapeA.GetCylinder();

		switch (shapeB.GetType())
		{
			case eShapeType::eSphere:
			{
				Sphere sphereB = shapeB.GetSphere();
				gjk.Setup((void*)&cylinderA, (void*)&sphereB, GetSupportVertexCylinder, GetSupportVertexSphere);
				err = gjk.Collide(d, normal, pA, pB, worldTransformA, worldTransformB);
				break;
			}
			case eShapeType::eBox:
			{
				Box boxB = shapeB.GetBox();
				gjk.Setup((void*)&cylinderA, (void*)&boxB, GetSupportVertexCylinder, GetSupportVertexBox);
				err = gjk.Collide(d, normal, pA, pB, worldTransformA, worldTransformB);
				break;
			}
			case eShapeType::eCapsule:
			{
				Capsule capsuleB = shapeB.GetCapsule();
				gjk.Setup((void*)&cylinderA, (void*)&capsuleB, GetSupportVertexCylinder, GetSupportVertexCapsule);
				err = gjk.Collide(d, normal, pA, pB, worldTransformA, worldTransformB);
				break;
			}
			case eShapeType::eCylinder:
			{
				Cylinder cylinderB = shapeB.GetCylinder();
				gjk.Setup((void*)&cylinderA, (void*)&cylinderB, GetSupportVertexCylinder, GetSupportVertexCylinder);
				err = gjk.Collide(d, normal, pA, pB, worldTransformA, worldTransformB);
				break;
			}
		}
	}
	else if (shapeB.GetType() == eShapeType::eCylinder)
	{
		Cylinder cylinderB = shapeB.GetCylinder();

		switch (shapeA.GetType())
		{
			case eShapeType::eSphere:
			{
				Sphere sphereA = shapeA.GetSphere();
				gjk.Setup((void*)&sphereA, (void*)&cylinderB, GetSupportVertexSphere, GetSupportVertexCylinder);
				err = gjk.Collide(d, normal, pA, pB, worldTransformA, worldTransformB);
				break;
			}
			case eShapeType::eBox:
			{
				Box boxA = shapeA.GetBox();
				gjk.Setup((void*)&boxA, (void*)&cylinderB, GetSupportVertexBox, GetSupportVertexCylinder);
				err = gjk.Collide(d, normal, pA, pB, worldTransformA, worldTransformB);

				break;
			}
			case eShapeType::eCapsule:
			{
				Capsule capsuleA = shapeA.GetCapsule();
				gjk.Setup((void*)&capsuleA, (void*)&cylinderB, GetSupportVertexCapsule, GetSupportVertexCylinder);
				err = gjk.Collide(d, normal, pA, pB, worldTransformA, worldTransformB);

				break;
			}

		}


	}

	if (err == eGjkResult::GjkOk && d < contactThreshold)
	{
		contacts.AddContactPoint(
			d,
			//法線をworldTransformAで回転
			GameMath::Rotate(normal, GameMath::Get3x3(worldTransformA)),
			//pAをoffsetTransformAで回転
			GameMath::Rotate(pA, offsetTransformA),
			//pBをworldTransformAで回転して、その結果をworldTransformBの逆行列で回転、
			//その結果をoffsetTransformBで回転
			GameMath::Rotate(GameMath::Rotate(GameMath::Rotate(pB, worldTransformA), GameMath::OrthoInverse3x4(worldTransformB)), offsetTransformB),
			SubData());
	}

}



//	Collision Detection Fanction Table
//  |          ０          |          １          |          ２          |          ３          |
//０|    球    ×    球    |    球    ×    箱    |    球    × カプセル |    球    ×   円柱   |
//１|    箱    ×    球    |    箱    ×    箱    |    箱    × カプセル |    箱    ×   円柱   |
//２| カプセル ×    球    | カプセル ×    箱    | カプセル × カプセル | カプセル ×   円柱   |
//３|   円柱   ×    球    |   円柱   ×    箱    |   円柱   × カプセル |   円柱   ×   円柱   |

DetectCollisionFunc funcTblDetectCollision[eShapeType::eShapeCount][eShapeType::eShapeCount] = {
	{ DetectCollisionSphereSphere, DetectCollisionSphereBox, DetectCollisionSphereCapsule, DetectCollisionGjk },
	{ DetectCollisionBoxSphere, DetectCollisionBoxBox, DetectCollisionBoxCapsule, DetectCollisionGjk },
	{ DetectCollisionCapsuleSphere, DetectCollisionCapsuleBox, DetectCollisionCapsuleCapsule, DetectCollisionGjk },
	{ DetectCollisionGjk, DetectCollisionGjk, DetectCollisionGjk, DetectCollisionGjk }
};

//	Collision Detection Function Table Interface
//	各剛体の形状に対応したあたり判定関数を返す
DetectCollisionFunc GetDetectCollisionFunc(UInt8 shapeTypeA, UInt8 shapeTypeB)
{
#ifdef _DEBUG
	if (shapeTypeA >= eShapeType::eShapeCount)
		return nullptr;

	if (shapeTypeB >= eShapeType::eShapeCount)
		return nullptr;
#endif

	return funcTblDetectCollision[shapeTypeA][shapeTypeB];

}


//あたり判定関数をセットする
//おそらく使用しない
int SetDetectCollisionFunc(UInt8 shapeTypeA, UInt8 shapeTypeB, DetectCollisionFunc func)
{
	if (shapeTypeA >= eShapeType::eShapeCount)
		return -1;

	if (shapeTypeB >= eShapeType::eShapeCount)
		return -1;

	funcTblDetectCollision[shapeTypeA][shapeTypeB] = func;

	return 0;
}