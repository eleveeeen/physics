#include"KeyframeAnimation.h"
#include"KeyframeFunc.h"
#define Check_LoopFlag(flag) flag&0x02		//2,3がループ
#define Check_ReverseFlag(flag) flag&0x01	//1,3が逆再生

int KeyframeManager::SetAnimation(const Keyframe* keys, const int* animframes, const int numKeys, const eKeyframeType type)
{
	//	idPoolにidが入っていないなら	 animの数をidにしてanimの数をインクリメント : idPoolに入っているidをもらってpoolの数をデクリメント
	int animID = (numAnimPool == 0) ? numAnimation++ : animPool[numAnimPool--];

	KeyframeList& list = animeList[animID];
	list.Reset();
	list.id = animID;

	//loopの場合先頭とつなげるために記憶
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

	//ループの場合
	if (Check_LoopFlag(type))
	{
		//先頭と末尾をつなげる
		end->ConnectNext(list.animeList);
	}

	//逆再生ならEnd通常再生ならNext
	list.nextKeyframe = (Check_ReverseFlag(type)) ? end : list.animeList->nextAnimation;

	list.nowKeyframe = list.animeList;


	list.type = type;

	//0ならsingle,それ以外ならcontinuous
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
		//循環しているからnowとendが一致したら終了
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
		//更新してアニメーションが終了したら
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
	//アニメーション情報が入っているか確認
	if (list.animeList)
	{
		return GetKeyframeFunc(list.format, list.type)(list);
	}
	return false;

}