#include"shape.h"
#include"Contact.h"
#include"GJK_Solver.h"
#include"GJK_SupportFunc.h"
#include"DetectCollisionFunc.h"

//�_�~�[�֐�
void DetectCollisionDummy(
	ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold)
{

}

//�{�b�N�X�ƃ{�b�N�X�̔���
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


	//�Փ˔��������ĐN���[�x�����炤
	float d = ContactBoxBox(normal, pA, pB, &boxA, worldTransformA, &boxB, worldTransformB);

	//�Փ˂��Ă�����ǉ�
	if (d < contactThreshold)
	{
		contacts.AddContactPoint(d, -normal, GameMath::Rotate(pA, offsetTransformA), 
			GameMath::Rotate(pB, offsetTransformB), SubData());
	}


}

//�{�b�N�X�ƃJ�v�Z���̔���
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

//�{�b�N�X�ƃX�t�B�A
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


//�J�v�Z���ƃ{�b�N�X
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

//�J�v�Z���ƃJ�v�Z��
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

//�J�v�Z���ƃX�t�B�A
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

//�X�t�B�A�ƃ{�b�N�X
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

//�X�t�B�A�ƃJ�v�Z��
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



//�X�t�B�A�ƃX�t�B�A
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

	//shapeA��shapeB�̌`��ɉ����Ă����ꂩ�̏����ɓ���
	//�V�����_�[���g���Ă��Ȃ��Ƃ��̊֐��ɓ���Ȃ�����ǂ��ɂ��q�b�g���Ȃ����Ƃ͂Ȃ��͂�

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
			//�@����worldTransformA�ŉ�]
			GameMath::Rotate(normal, GameMath::Get3x3(worldTransformA)),
			//pA��offsetTransformA�ŉ�]
			GameMath::Rotate(pA, offsetTransformA),
			//pB��worldTransformA�ŉ�]���āA���̌��ʂ�worldTransformB�̋t�s��ŉ�]�A
			//���̌��ʂ�offsetTransformB�ŉ�]
			GameMath::Rotate(GameMath::Rotate(GameMath::Rotate(pB, worldTransformA), GameMath::OrthoInverse3x4(worldTransformB)), offsetTransformB),
			SubData());
	}

}



//	Collision Detection Fanction Table
//  |          �O          |          �P          |          �Q          |          �R          |
//�O|    ��    �~    ��    |    ��    �~    ��    |    ��    �~ �J�v�Z�� |    ��    �~   �~��   |
//�P|    ��    �~    ��    |    ��    �~    ��    |    ��    �~ �J�v�Z�� |    ��    �~   �~��   |
//�Q| �J�v�Z�� �~    ��    | �J�v�Z�� �~    ��    | �J�v�Z�� �~ �J�v�Z�� | �J�v�Z�� �~   �~��   |
//�R|   �~��   �~    ��    |   �~��   �~    ��    |   �~��   �~ �J�v�Z�� |   �~��   �~   �~��   |

DetectCollisionFunc funcTblDetectCollision[eShapeType::eShapeCount][eShapeType::eShapeCount] = {
	{ DetectCollisionSphereSphere, DetectCollisionSphereBox, DetectCollisionSphereCapsule, DetectCollisionGjk },
	{ DetectCollisionBoxSphere, DetectCollisionBoxBox, DetectCollisionBoxCapsule, DetectCollisionGjk },
	{ DetectCollisionCapsuleSphere, DetectCollisionCapsuleBox, DetectCollisionCapsuleCapsule, DetectCollisionGjk },
	{ DetectCollisionGjk, DetectCollisionGjk, DetectCollisionGjk, DetectCollisionGjk }
};

//	Collision Detection Function Table Interface
//	�e���̂̌`��ɑΉ����������蔻��֐���Ԃ�
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


//�����蔻��֐����Z�b�g����
//�����炭�g�p���Ȃ�
int SetDetectCollisionFunc(UInt8 shapeTypeA, UInt8 shapeTypeB, DetectCollisionFunc func)
{
	if (shapeTypeA >= eShapeType::eShapeCount)
		return -1;

	if (shapeTypeB >= eShapeType::eShapeCount)
		return -1;

	funcTblDetectCollision[shapeTypeA][shapeTypeB] = func;

	return 0;
}