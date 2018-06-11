//------------------------
//衝突可能かどうかについてのクラス
//Easy Physicsを参考に作成
//------------------------

#ifndef _Collidable_h_
#define _Collidable_h_

#include"shape.h"
#include"aabb.h"

class __declspec(align(128)) Collidable
{
	friend class ShapeIterator;
private:
	Shape* shapeBase;	//オブジェクトの形状
	
	float center[3];	//原点　
	float half[3];		//半径
	Shape defShape;

	unsigned short shapeIds[64];
	unsigned char numShapes;
	unsigned char maxShapes;

private:
	inline Shape& GetNewShape();

public:
	inline void Reset();
	inline void Reset(Shape* base, unsigned short* ids, int n = 1);

	//オブジェクトの形状を確定する
	void Finish();

	//オブジェクト追加
	void AddShape(const Shape& shape);

	inline unsigned int GetNumShapes()const;
	inline unsigned int GetMaxNumShapes()const;
	const Shape& GetDefShape()const
	{
		return defShape;
	}
	inline unsigned short GetShapeID(int i)const;
	inline const Shape& GetShape(int i)const;
	inline Shape& GetShape(int i);

	inline void SetCenter(const D3DXVECTOR3& _center);
	inline void SetHalf(const D3DXVECTOR3& _half);

	inline D3DXVECTOR3 GetHalf()const;
	inline D3DXVECTOR3 GetCenter()const;

};

#include"CollidableImplementation.h"

#endif