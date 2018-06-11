#include"GameStage.h"


void GameStage::Initialize(float scale)
{




	SetModelID(4);//スカイドーム
	D3DXMatrixIdentity(&data.data.transform);

	D3DXMatrixScaling(&data.data.transform, scale, scale, scale);

}


void GameStage::Update()
{

	Draw();

}