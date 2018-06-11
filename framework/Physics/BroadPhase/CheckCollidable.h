#ifndef _CheckCollidable_h_
#define _CheckCollidable_h_

#include"../RigidBody/RigidState.h"
#include"../Collision/aabb.h"
#include"BroadPhaseProxy.h"
#include"BroadPhasePair.h"


//	collidableTable
//                             motionA
//         |  fixed  |  active  | keyframe|  oneway  | trigger |
//  fixed  |    0    |     1    |    0    |     1    |    1    |
//  active |    1    |     1    |    1    |     1    |    1    |
// keyframe|    0    |     1    |    0    |     1    |    1    |
//  oneway |    1    |     1    |    1    |     0    |    1    |
// trigger |    1    |     1    |    1    |     1    |    0    |
//    motionB
// 0000 0000 1011 1111 1010 1111 1011 1110

static inline bool CheckCollidableTable(eMotionType i, eMotionType j)
{
	const UInt32 collidableTable = 0x00bfafbe;

	UInt32 idx = j*eMotionType::eMotionCount + i;

	UInt32 mask = 1 << (eMotionType::eMotionCount*eMotionType::eMotionCount - 1 - idx);

	return (collidableTable&mask) != 0;
}

static inline bool CheckCollidableBroadphase(const BroadphaseProxy& proxyA, const BroadphaseProxy& proxyB)
{
	eMotionType motionA = (eMotionType)(GetMotionMask(proxyA)&MOTION_MASK_TYPE);
	eMotionType motionB = (eMotionType)(GetMotionMask(proxyB)&MOTION_MASK_TYPE);

	return CheckCollidableTable(motionA, motionB) &&										//モーションタイプ別衝突判定テーブル
		((GetSelf(proxyA)&GetTarget(proxyB)) && (GetTarget(proxyA)&GetSelf(proxyB))) &&		//衝突フィルター
		TestAabb(proxyA, proxyB);															//AABB判定
}

//aabbをする必要があるか判定
//静止している場合はfalseを返す
static inline bool CheckCollidableInCollision(const BroadphasePair& pair)
{
	UInt32 motionA = GetMotionMaskA(pair)&MOTION_MASK_TYPE;
	UInt32 motionB = GetMotionMaskB(pair)&MOTION_MASK_TYPE;

	UInt32 sleepA = GetMotionMaskA(pair)&MOTION_MASK_SLEEPING;
	UInt32 sleepB = GetMotionMaskB(pair)&MOTION_MASK_SLEEPING;

#ifdef _DEBUG
	bool check1 = CheckCollidableTable((eMotionType)motionA, (eMotionType)motionB);	//モーションタイプ別衝突判定テーブル
	bool check2 = (sleepA != 0 && sleepB != 0);						//静止している場合はtrue
	bool check3 = (sleepA != 0 && motionB == eMotionType::eFixed);	//静止している場合はtrue
	bool check4 = (sleepB != 0 && motionA == eMotionType::eFixed);	//静止している場合はtrue

	return check1 && !(check2 || check3 || check4);
#else

	return CheckCollidableTable((eMotionType)motionA, (eMotionType)motionB) &&																	//モーションタイプ別衝突判定テーブル
		!((sleepA != 0 && sleepB != 0) || (sleepA != 0 && motionB == eMotionType::eFixed) || (sleepB != 0 && motionA == eMotionType::eFixed));	//スリープ時のチェック
#endif
}


#endif