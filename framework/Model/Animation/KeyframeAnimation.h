#ifndef _KeyframeAnimation_h_
#define _KeyframeAnimation_h_
//�L�[�t���[���A�j���[�V�����Ɋւ���w�b�_�[
//�X���[�v���

#include<d3dx9.h>

//�A�j���[�V�����̃^�C�v
enum eKeyframeType
{
	KeyframeOnce = 0,		//1�񂾂��Đ�
	KeyframeReverseOnce,	//1�񂾂��t�Đ�	
	KeyframeLoop,			//���[�v�Đ�
	KeyframeReverseLoop,	//���[�v�t�Đ�
	KeyTypeCount
};//type��0x01xor���Ēʏ�Đ��Ƌt�Đ������R�ɐ؂�ւ����悤��


//�A�j���[�V�����̌`��
enum eKeyframeFormat
{
	KeyframeContinuous,		//�����̎p�����
	KeyframeSingle,			//�P�̂̎p�����
	KeyFormatCount

};

//�p�����
struct Keyframe
{
	D3DXQUATERNION rot;	//��]���
	D3DXVECTOR3 pos;	//���W���
};




struct KeyframeData
{
	Keyframe key;					//�p��
	int frame;						//�t���[��
	int id;
	KeyframeData* nextAnimation;	//���̃A�j���[�V�����@�Ȃ����null
	KeyframeData* prevAnimation;	//�O�̃A�j���[�V�����@�Ȃ����null

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
	Keyframe key;						//���݂̎p��
	eKeyframeType type;					//�^�C�v
	eKeyframeFormat format;				//�`��
	KeyframeData* animeList;			//�A�j���[�V�����̃��X�g
	const KeyframeData* nowKeyframe;	//�X���[�v�p�p��(t=0)
	const KeyframeData* nextKeyframe;	//�X���[�v�p�p��(t=1)
	int nowFrame;						//���݂̃t���[��
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
private://�萔��`
	//�L�[�t���[���̍ő吔
	static const int MaxKeyframe = 2000;
	//�A�j���[�V�����̍ő�o�^��
	static const int MaxAnimation = 500;
private:

	KeyframeData keyframes[MaxKeyframe];
	KeyframeList animeList[MaxAnimation];
	KeyframeList listTop;	//�擪�Ƀ_�~�[����������
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
	//�A�j���[�V�����o�^
	int SetAnimation(const Keyframe* keys, const int* animframes, const int numKeys, const eKeyframeType type);
	//�A�j���[�V��������
	void UnsetAnimation(const int animID);


	//�L�[�t���[�����擾
	const Keyframe& GetKeyframe(const int keyframeID)const
	{
		return keyframes[keyframeID].key;
	}

public://�X�V
	void Update();	//�o�^����Ă���A�j���[�V���������ׂčX�V

	//�w�肵��ID�̃A�j���[�V�������X�V
	//�A�j���[�V�����I�����A���s����false��Ԃ�
	bool Update(const int animID);

private:

};


#endif