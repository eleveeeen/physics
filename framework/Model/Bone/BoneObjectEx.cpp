#include"BoneObjectEx.h"


void BoneObjectEx::UpdateBone()
{
	if (!bone[0].physicsflag)
		bone[0].absolute = bone[0].relative;

	D3DXVECTOR3 rotatePos;
	for (int i = 1; i < numBones; ++i)
	{

		auto& my = bone[i];

		if (my.physicsflag)
			continue;

		//rootは別で処理しているから親がいない可能性はないはず
		const auto& parent = bone[my.tree.parentID];

		//親の空間に変換
		my.absolute.rot = my.relative.rot * parent.absolute.rot;
	
		rotatePos = GameMath::Rotate(my.relative.pos, parent.absolute.rot);
		my.absolute.pos = parent.absolute.pos + rotatePos;

	}

}


