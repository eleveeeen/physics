#ifndef _GJK_SupportFunc_h_
#define _GJK_SupportFunc_h_

#include"../Base/Common.h"

//GJK用の関数群
//すべて関数ポインタを使って使用


//SupportFunc 
void GetSupportVertexTriangle(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex);
void GetSupportVertexTriangleWithThickness(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex);
void GetSupportVertexConvex(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex);
void GetSupportVertexBox(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex);
void GetSupportVertexCapsule(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex);
void GetSupportVertexSphere(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex);
void GetSupportVertexCylinder(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex);

//Center
void GetCenterTriangle(void* shape, D3DXVECTOR3& center);
void GetCenterTriangleWithThickness(void* shape, D3DXVECTOR3& center);
void GetCenterConvex(void* shape, D3DXVECTOR3& center);
void GetCenterBox(void* shape, D3DXVECTOR3& center);
void GetCenterCapsule(void* shape, D3DXVECTOR3& center);
void GetCenterSphere(void* shape, D3DXVECTOR3& center);
void GetCenterCylinder(void* shape, D3DXVECTOR3& center);




#endif