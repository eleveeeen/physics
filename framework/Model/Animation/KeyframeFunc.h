#ifndef _KeyframeFunc_h_
#define _KeyframeFunc_h_

struct KeyframeList;


typedef bool(*KeyframeFunc)(KeyframeList& animData);

//�L�[�t���[���̃^�C�v�̊֐����擾
KeyframeFunc GetKeyframeFunc(const int format, const int type);
#endif