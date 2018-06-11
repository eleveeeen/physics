//------------------------
//�V�[���̊��N���X
//���z�N���X������p�����Ďg��
//------------------------
#ifndef _BaseScene_h_
#define _BaseScene_h_

class BaseScene
{

public:
	//�V�[���J�n����1�x�������鏈��
	virtual void SceneStart() = 0;
	//�V�[���I������1�x�������鏈��
	virtual void SceneEnd() = 0;

	//�����̏���������
	virtual int Initialize() = 0;
	//�����̏I������
	virtual int Finalize() = 0;
	//�����̍X�V����
	virtual BaseScene* Update() = 0;
	//�����̕`�揈��
	virtual void Render() = 0;
};





#endif