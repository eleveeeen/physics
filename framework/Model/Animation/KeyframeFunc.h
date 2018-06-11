#ifndef _KeyframeFunc_h_
#define _KeyframeFunc_h_

struct KeyframeList;


typedef bool(*KeyframeFunc)(KeyframeList& animData);

//キーフレームのタイプの関数を取得
KeyframeFunc GetKeyframeFunc(const int format, const int type);
#endif