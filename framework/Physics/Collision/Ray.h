#ifndef _Ray_h_
#define _Ray_h_
//���C���̃w�b�_�[
//�}�E�X�J�[�\���ŉ�ʂ̒���G�鎞�ȂǂɎg�p


#include"..\Base\Common.h"
#include"SubData.h"

//���C�L���X�g�̃��[�h
enum eRayFacetMode
{
	eFrontOnly=0,		//�O�̂�
	eBackOnly,			//���̂�
	eFrontAndBack,		//����

};


struct RayInput
{
	D3DXVECTOR3 startPos;
	D3DXVECTOR3 dir;
	UInt32		contactFilterSelf;
	UInt32		contactFilterTarget;
	UInt8		facetMode;

	void Reset()
	{
		contactFilterSelf = contactFilterTarget = 0xffffffff;
		facetMode = eRayFacetMode::eFrontOnly;
	}


};


struct RayOutput
{
	D3DXVECTOR3 contactPoint;
	D3DXVECTOR3 contactNormal;
	float		variable;
	UInt16		objectID;
	UInt8		shapeID;
	bool		contactFlag;
	SubData		subData;

};



#endif