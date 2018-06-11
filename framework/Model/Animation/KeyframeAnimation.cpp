#include"KeyframeAnimation.h"
#include"KeyframeFunc.h"
#define Check_LoopFlag(flag) flag&0x02		//2,3�����[�v
#define Check_ReverseFlag(flag) flag&0x01	//1,3���t�Đ�

int KeyframeManager::SetAnimation(const Keyframe* keys, const int* animframes, const int numKeys, const eKeyframeType type)
{
	//	idPool��id�������Ă��Ȃ��Ȃ�	 anim�̐���id�ɂ���anim�̐����C���N�������g : idPool�ɓ����Ă���id���������pool�̐����f�N�������g
	int animID = (numAnimPool == 0) ? numAnimation++ : animPool[numAnimPool--];

	KeyframeList& list = animeList[animID];
	list.Reset();
	list.id = animID;

	//loop�̏ꍇ�擪�ƂȂ��邽�߂ɋL��
	KeyframeData* end = nullptr;
	for (int i = 0; i < numKeys; ++i)
	{
		int keyID = (numKeyPool == 0) ? numKeyframes++ : keyPool[numKeyPool--];
		
		KeyframeData& key = keyframes[keyID];
		key.id = keyID;
		key.Set(keys[i], animframes[i]);
		
		if (end)
		{
			end->ConnectNext(&key);
			end = end->nextAnimation;
		}
		else
		{
			list.animeList = &key;
			end = &key;
		}

	}

	//���[�v�̏ꍇ
	if (Check_LoopFlag(type))
	{
		//�擪�Ɩ������Ȃ���
		end->ConnectNext(list.animeList);
	}

	//�t�Đ��Ȃ�End�ʏ�Đ��Ȃ�Next
	list.nextKeyframe = (Check_ReverseFlag(type)) ? end : list.animeList->nextAnimation;

	list.nowKeyframe = list.animeList;


	list.type = type;

	//0�Ȃ�single,����ȊO�Ȃ�continuous
	list.format = (eKeyframeFormat)(!numKeys);



	if (listTop.next)
	{
		listEnd->next = &list;
		listEnd = listEnd->next;
	}
	else
	{
		listTop.next = &list;
		listEnd = &list;
	}
	

	return animID;
}
void KeyframeManager::UnsetAnimation(const int animID)
{
	KeyframeList* prev = animeList[animID].prev;
	KeyframeList* next = animeList[animID].next;

	prev->next = next;
	if (next)
	{
		next->prev = prev;
	}

	animPool[numAnimPool++] = animID;
	if (Check_LoopFlag(animeList[animID].type))
	{
		//�z���Ă��邩��now��end����v������I��
		auto now = animeList[animID].animeList;
		auto end = now;
		
		do
		{
			keyPool[numKeyPool++] = now->id;
			now = now->nextAnimation;
		} while (now != end);
	}
	else
	{
		for (auto now = animeList[animID].animeList; now; now = now->nextAnimation)
		{
			keyPool[numKeyPool++] = now->id;
		}

	}

	animeList[animID].Reset();
}


void KeyframeManager::Update()
{
	for (auto now = listTop.next; now;)
	{
		//�X�V���ăA�j���[�V�������I��������
		if (!GetKeyframeFunc(now->format, now->type)(*now))
		{
			int delID = now->id;
			now = now->next;
			UnsetAnimation(delID);
			
		}
		else
		{
			now = now->next;
		}
	}


}

bool KeyframeManager::Update(const int animID)
{

	KeyframeList& list = animeList[animID];
	//�A�j���[�V������񂪓����Ă��邩�m�F
	if (list.animeList)
	{
		return GetKeyframeFunc(list.format, list.type)(list);
	}
	return false;

}