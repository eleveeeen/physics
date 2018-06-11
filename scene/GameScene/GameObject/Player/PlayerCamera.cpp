#include"PlayerCamera.h"
#include"../../../../framework/Utility/GameMath.h"

//�A�b�v�f�[�g���ɌĂяo���֐��e�[�u��
PlayerCamera::AnimFunc PlayerCamera::funcAnimTable[PlayerCamera::COUNT] =
{
	&PlayerCamera::Wait,
	&PlayerCamera::Rotate,
	&PlayerCamera::Move,
	&PlayerCamera::Transform
};

//���̃J�������Z�b�g
void SetNextCamera(const D3DXQUATERNION& newDir, const D3DXVECTOR3& newPos);

//������
void PlayerCamera::Init(const D3DXVECTOR3& cameraPos, const D3DXVECTOR3& viewPos)
{

	//return;

	state = WAIT;
	
	pos = cameraPos;
	
	quake = D3DXQUATERNION(0, 0, 0, 1);

	//�J�����̌����v�Z
	//���ꂼ��̎������߂čs��ɂ��Č�̓w���p�[�C��

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
	//�J�����̈ړ�
	(this->*funcAnimTable[state])();


	//�J�������X�V���邩��s��̌`��
	D3DXMATRIX mat;
	D3DXMatrixRotationQuaternion(&mat, &(quake*dir));
	mat._41 = pos.x;
	mat._42 = pos.y;
	mat._43 = pos.z;

	GameCamera::GetInstance()->SetCamera(mat);


	
}

//���̎p���Z�b�g
void PlayerCamera::SetNextCamera(const D3DXQUATERNION& nextDir, const D3DXVECTOR3& nextPos, int frame)
{
	//ID�擾
	int id = GetTaskID();

	//�ő吔�I�[�o�[
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

//���̍��W�Z�b�g
void PlayerCamera::SetNextPos(const D3DXVECTOR3& nextPos, int frame)
{
	//ID�擾
	int id = GetTaskID();

	//�ő吔�I�[�o�[
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

//���̌����Z�b�g
void PlayerCamera::SetNextDir(const D3DXQUATERNION& nextDir, int frame)
{
	//ID�擾
	int id = GetTaskID();

	//�ő吔�I�[�o�[
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

//�ҋ@
void PlayerCamera::Wait()
{
	if (targetTask)
	{
		SetupTask();
		(this->*funcAnimTable[state])();
	}

}

//��]
//���ʐ��`���
void PlayerCamera::Rotate()
{
	float t = (float)++count / (float)targetTask->frame;

	D3DXQUATERNION q1 = oldDir;			//�ړ��O�̎p��
	D3DXQUATERNION q2(targetTask->f);	//�ړ���̎p��


	D3DXQuaternionSlerp(&dir, &q1, &q2, t);

	if (count == targetTask->frame)
	{
		NextTarget();
	}



}

//�ړ�
//���`���
void PlayerCamera::Move()
{
	float t = (float)++count / (float)targetTask->frame;

	D3DXVECTOR3 v1 = oldPos;		//�ړ��O�̍��W
	D3DXVECTOR3 v2(targetTask->f);	//�ړ���̍��W
	
	pos = GameMath::Lerp(v1, v2, t);


	if (count == targetTask->frame)
	{
		NextTarget();
	}

}

//��]���ړ�
//���ʐ��`���
void PlayerCamera::Transform()
{
	float t = (float)++count / (float)targetTask->frame;

	D3DXVECTOR3 v1 = oldPos;			//�ړ��O�̍��W
	D3DXVECTOR3 v2(targetTask->pos);	//�ړ���̍��W

	D3DXQUATERNION q1 = oldDir;			//�ړ��O�̎p��
	D3DXQUATERNION q2(targetTask->dir);	//�ړ���̎p��


	pos = GameMath::Slerp(v1, v2, t);
	D3DXQuaternionSlerp(&dir, &q1, &q2, t);

	if (count == targetTask->frame)
	{
		NextTarget();
	}
	

}
