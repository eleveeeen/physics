#ifndef _DetectCollisionFunc_h_
#define _DetectCollisionFunc_h_

#include"ContactCache.h"
#include"shape.h"

typedef void(*DetectCollisionFunc)(
	ContactCache& contacts,
	const Shape& shapeA, const D3DXMATRIX& offsetTransformA, const D3DXMATRIX& worldTransformA, int shapeIDA,
	const Shape& shapeB, const D3DXMATRIX& offsetTransformB, const D3DXMATRIX& worldTransformB, int shapeIDB,
	float contactThreshold);


DetectCollisionFunc GetDetectCollisionFunc(UInt8 shapeTypeA, UInt8 shapeTypeB);

int SetDetectCollisionFunc(UInt8 shapeTypeA, UInt8 shapeTypeB, DetectCollisionFunc func);

#endif