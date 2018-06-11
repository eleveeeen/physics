#pragma once

#include"../../../../framework/GameManager/GameCamera.h"

class PlayerCamera
{
private://�萔��`
	enum State
	{
		WAIT = 0,	//�ړ��Ȃ�
		ROTATE,		//��]��
		MOVE,		//�ړ���
		TRANSFORM,	//�ړ�����]
		COUNT,		//����
	};

	static const int TASKMAX = 50;	//�^�X�N�̍ő吔

private://�^��`

	//�J�����̃^�X�N
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

private://�֐��|�C���^

	typedef void(PlayerCamera::*AnimFunc)();

	//�J�����̃A�j���[�V�����p�̊֐��e�[�u��
	static AnimFunc funcAnimTable[COUNT];


private://�����o�ϐ�

	//�J�����̃f�[�^
	D3DXQUATERNION dir;
	D3DXVECTOR3 pos;

	//�J�����̍s���������
	CameraTask* targetTask;
	CameraTask* taskEnd;

	//�^�X�N�̎���
	CameraTask taskData[TASKMAX];

	int numTask;//���ݐς܂�Ă���^�X�N�̐�

	int idPool[TASKMAX];
	int numPool;

	//�^�X�N�J�n���̏��i�[�p
	D3DXQUATERNION oldDir;
	D3DXVECTOR3 oldPos;

	State state;
	int count;


	D3DXQUATERNION quake;

public://�����o�֐�

	PlayerCamera()
	{
		targetTask = nullptr;
		taskEnd = nullptr;
		SecureZeroMemory(taskData, sizeof(CameraTask)*TASKMAX);
		SecureZeroMemory(idPool, sizeof(int)*TASKMAX);
		numTask = 0;
		numPool = 0;

	}

	//�J�����̏�����
	void Init(const D3DXVECTOR3& cameraPos, const D3DXVECTOR3& viewPos);

	//���̃J�������Z�b�g
	void SetNextCamera(const D3DXQUATERNION& nextDir, const D3DXVECTOR3& nextPos, int frame = 20);
	void SetNextPos(const D3DXVECTOR3& nextPos, int frame = 20);
	void SetNextDir(const D3DXQUATERNION& nextDir, int frame = 20);

	//�J�����̗h��ݒ�
	void Quake(D3DXQUATERNION& q)
	{
		quake = q;
	}

	//�J�����̍X�V
	void Update();

	//�J�����̕����擾
	D3DXQUATERNION GetDirection()const
	{
		return dir;
	}

	//�J�������W�擾
	D3DXVECTOR3 GetPosition()const
	{
		return pos;
	}



private://�����o�֐�


	//�ړ��Ȃ�
	void Wait();
	//��]��
	void Rotate();
	//�ړ���
	void Move();
	//�ړ�����]
	void Transform();


	//�^�X�N�̊J�n����
	void SetupTask()
	{
		count = 0;
		state = targetTask->state;
		oldPos = pos;
		oldDir = dir;

	}

	//���̃^�[�Q�b�g��
	void NextTarget()
	{
		//ID���v�[���ɓ����next��
		idPool[numPool++] = targetTask->id;
		int id = targetTask->id;
		targetTask = targetTask->next;
		//�܂��^�X�N���c���Ă���Ȃ�
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

	//�z��ԍ��擾
	int GetTaskID()
	{
		return (numPool == 0) ?
			numTask++ :
			idPool[--numPool];
	}

};

