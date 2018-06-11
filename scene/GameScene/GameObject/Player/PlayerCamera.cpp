#include"PlayerCamera.h"
#include"../../../../framework/Utility/GameMath.h"

//アップデート時に呼び出す関数テーブル
PlayerCamera::AnimFunc PlayerCamera::funcAnimTable[PlayerCamera::COUNT] =
{
	&PlayerCamera::Wait,
	&PlayerCamera::Rotate,
	&PlayerCamera::Move,
	&PlayerCamera::Transform
};

//次のカメラ情報セット
void SetNextCamera(const D3DXQUATERNION& newDir, const D3DXVECTOR3& newPos);

//初期化
void PlayerCamera::Init(const D3DXVECTOR3& cameraPos, const D3DXVECTOR3& viewPos)
{

	//return;

	state = WAIT;
	
	pos = cameraPos;
	
	quake = D3DXQUATERNION(0, 0, 0, 1);

	//カメラの向き計算
	//それぞれの軸を求めて行列にして後はヘルパー任せ

	D3DXVECTOR3 zAxis = viewPos - cameraPos;
	D3DXVec3Normalize(&zAxis, &zAxis);
	D3DXVECTOR3 xAxis;
	D3DXVec3Cross(&xAxis, &D3DXVECTOR3(0, 1, 0), &zAxis);
	D3DXVec3Normalize(&xAxis, &xAxis);
	D3DXVECTOR3 yAxis;
	D3DXVec3Cross(&yAxis, &zAxis, &xAxis);

	D3DXMATRIX v;
	D3DXMatrixIdentity(&v);
	v._11 = xAxis.x;
	v._12 = xAxis.y;
	v._13 = xAxis.z;
	v._21 = yAxis.x;
	v._22 = yAxis.y;
	v._23 = yAxis.z;
	v._31 = zAxis.x;
	v._32 = zAxis.y;
	v._33 = zAxis.z;

	D3DXQuaternionRotationMatrix(&dir, &v);



}


void PlayerCamera::Update()
{
	//カメラの移動
	(this->*funcAnimTable[state])();


	//カメラを更新するから行列の形に
	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, &(quake*dir));
	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;

	GameCamera::GetInstance()->SetCamera(mat);


	
}

//次の姿勢セット
void PlayerCamera::SetNextCamera(const D3DXQUATERNION& nextDir, const D3DXVECTOR3& nextPos, int frame)
{
	//ID取得
	int id = GetTaskID();

	//最大数オーバー
	if (id == TASKMAX)
	{
		--numTask;
		return;
	}

	if (targetTask == nullptr)
	{
		taskEnd = targetTask = &taskData[id];

	}
	else
	{
		taskEnd->next = &taskData[id];
		taskEnd = taskEnd->next;
	}

	memcpy(taskData[id].pos, nextPos, sizeof(float) * 3);
	memcpy(taskData[id].dir, nextDir, sizeof(float) * 4);

	taskData[id].frame = frame;
	taskData[id].id = id;

	taskData[id].state = State::TRANSFORM;

	taskData[id].next = nullptr;

}

//次の座標セット
void PlayerCamera::SetNextPos(const D3DXVECTOR3& nextPos, int frame)
{
	//ID取得
	int id = GetTaskID();

	//最大数オーバー
	if (id == TASKMAX)
	{
		--numTask;
		return;
	}

	if (targetTask == nullptr)
	{
		taskEnd = targetTask = &taskData[id];
		
	}
	else
	{
		taskEnd->next = &taskData[id];
		taskEnd = taskEnd->next;
	}

	taskData[id].x = nextPos.x;
	taskData[id].y = nextPos.y;
	taskData[id].z = nextPos.z;

	taskData[id].frame = frame;
	taskData[id].id = id;

	taskData[id].state = State::MOVE;

	taskData[id].next = nullptr;


}

//次の向きセット
void PlayerCamera::SetNextDir(const D3DXQUATERNION& nextDir, int frame)
{
	//ID取得
	int id = GetTaskID();

	//最大数オーバー
	if (id == TASKMAX)
	{
		--numTask;
		return;
	}

	if (targetTask == nullptr)
	{
		taskEnd = targetTask = &taskData[id];

	}
	else
	{
		taskEnd->next = &taskData[id];
		taskEnd = taskEnd->next;
	}

	taskData[id].x = nextDir.x;
	taskData[id].y = nextDir.y;
	taskData[id].z = nextDir.z;
	taskData[id].w = nextDir.w;

	taskData[id].frame = frame;
	taskData[id].id = id;

	taskData[id].state = State::ROTATE;

	taskData[id].next = nullptr;
}

//待機
void PlayerCamera::Wait()
{
	if (targetTask)
	{
		SetupTask();
		(this->*funcAnimTable[state])();
	}

}

//回転
//球面線形補間
void PlayerCamera::Rotate()
{
	float t = (float)++count / (float)targetTask->frame;

	D3DXQUATERNION q1 = oldDir;			//移動前の姿勢
	D3DXQUATERNION q2(targetTask->f);	//移動後の姿勢


	D3DXQuaternionSlerp(&dir, &q1, &q2, t);

	if (count == targetTask->frame)
	{
		NextTarget();
	}



}

//移動
//線形補間
void PlayerCamera::Move()
{
	float t = (float)++count / (float)targetTask->frame;

	D3DXVECTOR3 v1 = oldPos;		//移動前の座標
	D3DXVECTOR3 v2(targetTask->f);	//移動後の座標
	
	pos = GameMath::Lerp(v1, v2, t);


	if (count == targetTask->frame)
	{
		NextTarget();
	}

}

//回転＆移動
//球面線形補間
void PlayerCamera::Transform()
{
	float t = (float)++count / (float)targetTask->frame;

	D3DXVECTOR3 v1 = oldPos;			//移動前の座標
	D3DXVECTOR3 v2(targetTask->pos);	//移動後の座標

	D3DXQUATERNION q1 = oldDir;			//移動前の姿勢
	D3DXQUATERNION q2(targetTask->dir);	//移動後の姿勢


	pos = GameMath::Slerp(v1, v2, t);
	D3DXQuaternionSlerp(&dir, &q1, &q2, t);

	if (count == targetTask->frame)
	{
		NextTarget();
	}
	

}
