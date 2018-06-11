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

		//root�͕ʂŏ������Ă��邩��e�����Ȃ��\���͂Ȃ��͂�
		const auto& parent = bone[my.tree.parentID];

		//�e�̋�Ԃɕϊ�
		my.absolute.rot = my.relative.rot * parent.absolute.rot;
	
		rotatePos = GameMath::Rotate(my.relative.pos, parent.absolute.rot);
		my.absolute.pos = parent.absolute.pos + rotatePos;

	}

}


