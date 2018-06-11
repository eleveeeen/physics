#include"KeyframeFunc.h"
#include"KeyframeAnimation.h"




inline void UpdateKeyframe(Keyframe& key, const KeyframeData& t0, const KeyframeData& t1,const float t)
{
	D3DXQuaternionSlerp(&key.rot, &t0.key.rot, &t1.key.rot, t);
	key.pos = t0.key.pos + ((t1.key.pos - t0.key.pos)*t);
}

//Onceはアニメーションの終了時にfalseを返す
//Loopは終了しないからtrueしか返さない



//単体＆１回再生  
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

//単体＆１回逆再生
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

//単体＆ループ再生 
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

//単体＆ループ逆再生
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


//複数＆１回再生  
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

//複数＆１回逆再生
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

//複数＆ループ再生 
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

//複数＆ループ逆再生
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
//　|          ０          |          １          |          ２          |          ３          |
//０|    複数＆１回再生    |   複数＆１回逆再生   |   複数＆ループ再生   |  複数＆ループ逆再生  |
//１|    単体＆１回再生    |   単体＆１回逆再生   |   単体＆ループ再生   |  単体＆ループ逆再生  |
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