#include"Collidable.h"
#include"../../Utility/GameMath.h"

//
void Collidable::AddShape(const Shape& shape)
{
	if (numShapes < maxShapes)
	{
		Shape& newShape = GetNewShape();
		newShape = shape;
		
	}


}

//形状確定
void Collidable::Finish()
{
	//オブジェクトがあれば原点と半径再設定

	if (numShapes == 0)
		return;

	D3DXVECTOR3 halfMax, halfMin;
	halfMax = GameMath::GetVec3(-FLT_MAX);
	halfMin = GameMath::GetVec3(FLT_MAX);

	
	for (UInt32 i = 0; i < GetNumShapes(); ++i)
	{
		const Shape& shape = GetShape(i);
		this->shapeIds[i] = shape.GetType();
		
		D3DXVECTOR3 aabbMin, aabbMax;
		shape.GetAabb(aabbMin, aabbMax);
		halfMax = GameMath::MaxPerElem(halfMax, aabbMax);
		halfMin = GameMath::MinPerElem(halfMin, aabbMin);
	}

	D3DXVECTOR3 allCenter = ((halfMin + halfMax)*0.5f);
	D3DXVECTOR3 allHalf = ((halfMax - halfMin)*0.5f);

	SetCenter(allCenter);
	SetHalf(allHalf);

}