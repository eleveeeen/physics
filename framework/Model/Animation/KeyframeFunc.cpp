#include"KeyframeFunc.h"
#include"KeyframeAnimation.h"




inline void UpdateKeyframe(Keyframe& key, const KeyframeData& t0, const KeyframeData& t1,const float t)
{
	D3DXQuaternionSlerp(&key.rot, &t0.key.rot, &t1.key.rot, t);
	key.pos = t0.key.pos + ((t1.key.pos - t0.key.pos)*t);
}

//Once�̓A�j���[�V�����̏I������false��Ԃ�
//Loop�͏I�����Ȃ�����true�����Ԃ��Ȃ�



//�P�́��P��Đ�  
bool KeyframeSingleOnce(KeyframeList& animData)
{
	++animData.nowFrame;
	const KeyframeData& t0 = *animData.nowKeyframe;
	const KeyframeData& t1 = *animData.nextKeyframe;

	if (animData.nowFrame == t1.frame)
	{
		animData.key = t1.key;
		return false;
	}
	else
	{
		UpdateKeyframe(animData.key, t0, t1, (float)t1.frame - (float)animData.nowFrame);
	}

	return true;
}

//�P�́��P��t�Đ�
bool KeyframeSingleReverseOnce(KeyframeList& animData)
{
	++animData.nowFrame;
	const KeyframeData& t0 = *animData.nowKeyframe;
	const KeyframeData& t1 = *animData.nextKeyframe;

	if (animData.nowFrame == t0.frame)
	{
		animData.key = t1.key;
		return false;
	}
	else
	{
		UpdateKeyframe(animData.key, t0, t1, (float)t0.frame - (float)animData.nowFrame);
	}

	return true;
}

//�P�́����[�v�Đ� 
bool KeyframeSingleLoop(KeyframeList& animData)
{
	++animData.nowFrame;
	const KeyframeData& t0 = *animData.nowKeyframe;
	const KeyframeData& t1 = *animData.nextKeyframe;

	if (animData.nowFrame == t1.frame)
	{
		animData.key = t0.key;
		animData.nowFrame = 0;
		
	}
	else
	{
		UpdateKeyframe(animData.key, t0, t1, (float)t1.frame - (float)animData.nowFrame);
	}
	return true;
}

//�P�́����[�v�t�Đ�
bool KeyframeSingleReverseLoop(KeyframeList& animData)
{
	++animData.nowFrame;
	const KeyframeData& t0 = *animData.nowKeyframe;
	const KeyframeData& t1 = *animData.nextKeyframe;

	if (animData.nowFrame == t0.frame)
	{
		animData.key = t0.key;
		animData.nowFrame = 0;
	}
	else
	{
		UpdateKeyframe(animData.key, t0, t1, (float)t0.frame - (float)animData.nowFrame);
	}
	return true;
}


//�������P��Đ�  
bool KeyframeContinuousOnce(KeyframeList& animData)
{

	++animData.nowFrame;
	const KeyframeData& t0 = *animData.nowKeyframe;
	const KeyframeData& t1 = *animData.nextKeyframe;

	if (animData.nowFrame == t1.frame)
	{
		animData.key = t1.key;
		if (t1.nextAnimation)
		{
			animData.nowFrame = 0;
			animData.nowKeyframe = &t1;
			animData.nextKeyframe = t1.nextAnimation;
		}
		else
			return false;
	
	}
	else
	{
		UpdateKeyframe(animData.key, t0, t1, (float)t1.frame - (float)animData.nowFrame);
	}

	return true;
}

//�������P��t�Đ�
bool KeyframeContinuousReverseOnce(KeyframeList& animData)
{

	++animData.nowFrame;
	const KeyframeData& t0 = *animData.nowKeyframe;
	const KeyframeData& t1 = *animData.nextKeyframe;

	if (animData.nowFrame == t0.frame)
	{
		animData.key = t1.key;
		if (t1.prevAnimation)
		{
			animData.nowFrame = 0;
			animData.nowKeyframe = &t1;
			animData.nextKeyframe = t1.prevAnimation;
		}
		else
			return false;
	}
	else
	{
		UpdateKeyframe(animData.key, t0, t1, (float)t0.frame - (float)animData.nowFrame);
	}

	return true;
}

//���������[�v�Đ� 
bool KeyframeContinuousLoop(KeyframeList& animData)
{
	++animData.nowFrame;
	const KeyframeData& t0 = *animData.nowKeyframe;
	const KeyframeData& t1 = *animData.nextKeyframe;

	if (animData.nowFrame == t1.frame)
	{
		animData.key = t1.key;

		animData.nowFrame = 0;
		animData.nowKeyframe = &t1;
		animData.nextKeyframe = t1.nextAnimation;


	}
	else
	{
		UpdateKeyframe(animData.key, t0, t1, (float)t1.frame - (float)animData.nowFrame);
	}
	return true;
}

//���������[�v�t�Đ�
bool KeyframeContinuousReverseLoop(KeyframeList& animData)
{

	++animData.nowFrame;
	const KeyframeData& t0 = *animData.nowKeyframe;
	const KeyframeData& t1 = *animData.nextKeyframe;

	if (animData.nowFrame == t0.frame)
	{
		animData.key = t1.key;

		animData.nowFrame = 0;
		animData.nowKeyframe = &t1;
		animData.nextKeyframe = t1.prevAnimation;

	}
	else
	{
		UpdateKeyframe(animData.key, t0, t1, (float)t0.frame - (float)animData.nowFrame);
	}

	return true;
}





//keyframe function table
//												type
//�@|          �O          |          �P          |          �Q          |          �R          |
//�O|    �������P��Đ�    |   �������P��t�Đ�   |   ���������[�v�Đ�   |  ���������[�v�t�Đ�  |
//�P|    �P�́��P��Đ�    |   �P�́��P��t�Đ�   |   �P�́����[�v�Đ�   |  �P�́����[�v�t�Đ�  |
//format
static KeyframeFunc funcTblKeyframe[eKeyframeFormat::KeyFormatCount][eKeyframeType::KeyTypeCount]=
{
	{ KeyframeContinuousOnce, KeyframeContinuousReverseOnce, KeyframeContinuousLoop, KeyframeContinuousReverseLoop },
	{ KeyframeSingleOnce, KeyframeSingleReverseOnce, KeyframeSingleLoop, KeyframeSingleReverseLoop },
};


KeyframeFunc GetKeyframeFunc(const int format, const int type)
{
	return funcTblKeyframe[format][type];
}