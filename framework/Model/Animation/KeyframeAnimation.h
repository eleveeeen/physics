#ifndef _KeyframeAnimation_h_
#define _KeyframeAnimation_h_
//キーフレームアニメーションに関するヘッダー
//スラープ補間

#include<d3dx9.h>

//アニメーションのタイプ
enum eKeyframeType
{
	KeyframeOnce = 0,		//1回だけ再生
	KeyframeReverseOnce,	//1回だけ逆再生	
	KeyframeLoop,			//ループ再生
	KeyframeReverseLoop,	//ループ逆再生
	KeyTypeCount
};//typeと0x01xorして通常再生と逆再生を自由に切り替えれるように


//アニメーションの形式
enum eKeyframeFormat
{
	KeyframeContinuous,		//複数の姿勢情報
	KeyframeSingle,			//単体の姿勢情報
	KeyFormatCount

};

//姿勢情報
struct Keyframe
{
	D3DXQUATERNION rot;	//回転情報
	D3DXVECTOR3 pos;	//座標情報
};




struct KeyframeData
{
	Keyframe key;					//姿勢
	int frame;						//フレーム
	int id;
	KeyframeData* nextAnimation;	//次のアニメーション　なければnull
	KeyframeData* prevAnimation;	//前のアニメーション　なければnull

	void Set(const Keyframe& _key, const int _frame)
	{
		key = _key;
		frame = _frame;
		nextAnimation = nullptr;
		prevAnimation = nullptr;
	}
	void ConnectNext(KeyframeData* next)
	{
		nextAnimation = next;
		next->prevAnimation = this;
	}
	void ConnectPrev(KeyframeData* prev)
	{
		prevAnimation = prev;
		prev->nextAnimation = this;
	}
};

struct KeyframeList
{
	Keyframe key;						//現在の姿勢
	eKeyframeType type;					//タイプ
	eKeyframeFormat format;				//形式
	KeyframeData* animeList;			//アニメーションのリスト
	const KeyframeData* nowKeyframe;	//スラープ用姿勢(t=0)
	const KeyframeData* nextKeyframe;	//スラープ用姿勢(t=1)
	int nowFrame;						//現在のフレーム
	int id;
	KeyframeList* next;
	KeyframeList* prev;

	void Reset()
	{
		animeList = nullptr;
		nowKeyframe = nullptr;
		nextKeyframe = nullptr;
		next = nullptr;
		prev = nullptr;
		nowFrame = 0;
	}
};

class KeyframeManager
{
private://定数定義
	//キーフレームの最大数
	static const int MaxKeyframe = 2000;
	//アニメーションの最大登録数
	static const int MaxAnimation = 500;
private:

	KeyframeData keyframes[MaxKeyframe];
	KeyframeList animeList[MaxAnimation];
	KeyframeList listTop;	//先頭にダミーを持たせる
	KeyframeList* listEnd;
	int numKeyframes;
	int numAnimation;
	int keyPool[MaxKeyframe];
	int numKeyPool;
	int animPool[MaxAnimation];
	int numAnimPool;


public:
	KeyframeManager()
	{
		numKeyframes = 0;
		numAnimation = 0;
		numKeyPool = 0;
		numAnimPool = 0;
		listTop.Reset();
	}
	//アニメーション登録
	int SetAnimation(const Keyframe* keys, const int* animframes, const int numKeys, const eKeyframeType type);
	//アニメーション解除
	void UnsetAnimation(const int animID);


	//キーフレーム情報取得
	const Keyframe& GetKeyframe(const int keyframeID)const
	{
		return keyframes[keyframeID].key;
	}

public://更新
	void Update();	//登録されているアニメーションをすべて更新

	//指定したIDのアニメーションを更新
	//アニメーション終了時、失敗時にfalseを返す
	bool Update(const int animID);

private:

};


#endif