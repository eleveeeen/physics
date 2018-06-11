#pragma once

#include"../../../../framework/GameManager/GameCamera.h"

class PlayerCamera
{
private://定数定義
	enum State
	{
		WAIT = 0,	//移動なし
		ROTATE,		//回転中
		MOVE,		//移動中
		TRANSFORM,	//移動＆回転
		COUNT,		//総数
	};

	static const int TASKMAX = 50;	//タスクの最大数

private://型定義

	//カメラのタスク
	struct CameraTask
	{
		union
		{
			float f[4];
			struct
			{
				float x, y, z, w;
			};
			struct
			{
				float dir[4];
				float pos[4];
			};
		};
		State state;
		int frame;
		int id;
		CameraTask* next;

	};

private://関数ポインタ

	typedef void(PlayerCamera::*AnimFunc)();

	//カメラのアニメーション用の関数テーブル
	static AnimFunc funcAnimTable[COUNT];


private://メンバ変数

	//カメラのデータ
	D3DXQUATERNION dir;
	D3DXVECTOR3 pos;

	//カメラの行う処理情報
	CameraTask* targetTask;
	CameraTask* taskEnd;

	//タスクの実体
	CameraTask taskData[TASKMAX];

	int numTask;//現在積まれているタスクの数

	int idPool[TASKMAX];
	int numPool;

	//タスク開始時の情報格納用
	D3DXQUATERNION oldDir;
	D3DXVECTOR3 oldPos;

	State state;
	int count;


	D3DXQUATERNION quake;

public://メンバ関数

	PlayerCamera()
	{
		targetTask = nullptr;
		taskEnd = nullptr;
		SecureZeroMemory(taskData, sizeof(CameraTask)*TASKMAX);
		SecureZeroMemory(idPool, sizeof(int)*TASKMAX);
		numTask = 0;
		numPool = 0;

	}

	//カメラの初期化
	void Init(const D3DXVECTOR3& cameraPos, const D3DXVECTOR3& viewPos);

	//次のカメラ情報セット
	void SetNextCamera(const D3DXQUATERNION& nextDir, const D3DXVECTOR3& nextPos, int frame = 20);
	void SetNextPos(const D3DXVECTOR3& nextPos, int frame = 20);
	void SetNextDir(const D3DXQUATERNION& nextDir, int frame = 20);

	//カメラの揺れ設定
	void Quake(D3DXQUATERNION& q)
	{
		quake = q;
	}

	//カメラの更新
	void Update();

	//カメラの方向取得
	D3DXQUATERNION GetDirection()const
	{
		return dir;
	}

	//カメラ座標取得
	D3DXVECTOR3 GetPosition()const
	{
		return pos;
	}



private://メンバ関数


	//移動なし
	void Wait();
	//回転中
	void Rotate();
	//移動中
	void Move();
	//移動＆回転
	void Transform();


	//タスクの開始準備
	void SetupTask()
	{
		count = 0;
		state = targetTask->state;
		oldPos = pos;
		oldDir = dir;

	}

	//次のターゲットへ
	void NextTarget()
	{
		//IDをプールに入れてnextに
		idPool[numPool++] = targetTask->id;
		int id = targetTask->id;
		targetTask = targetTask->next;
		//まだタスクが残っているなら
		if (targetTask)
		{
			SetupTask();
		}
		else
		{
			state = State::WAIT;
		}

		ZeroMemory(&taskData[id], sizeof(CameraTask));
	}

	//配列番号取得
	int GetTaskID()
	{
		return (numPool == 0) ?
			numTask++ :
			idPool[--numPool];
	}

};

