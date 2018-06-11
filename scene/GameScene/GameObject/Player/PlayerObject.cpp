#include"PlayerObject.h"

#include"../../../../framework/ResourceManager/Model/ModelManager.h"
#include"../../../../framework/ResourceManager/Bone/BoneManager.h"
#include"../../../../framework/ResourceManager/Material/MaterialManager.h"
#include"../../../../framework/Input/CInputKeyboard.h"
#include"../../../../framework/Input/CInputMouse.h"

#include"../../../../framework/GameManager/GameAnimation.h"
#include"../../../../framework/Graphics/CubeMap/CubeMapManager.h"
#include"../../../../framework/GameManager/GameLight.h"

PlayerObject::PlayerObject()
{


}

PlayerObject::~PlayerObject()
{
	
}

void PlayerObject::Initialize()
{


	//カメラ初期化
	const float startY = 1.0f;					//初期の高さ
	const D3DXVECTOR3 pos(0, startY, -8.0f);	//カメラ座標
	const D3DXVECTOR3 target(0, startY, 0);		//注視点
	camera.Init(pos, target);

	oldPos = camera.GetPosition();

	

	RigidParam param;

	const float radius = 0.5f;			//半径
	const float mass = 0.6f;			//質量　体積の関係上ラグドールに簡単に跳ね返される程度の質量
	const float restitution = 0.4f;		//反発係数
	const float friction = 0.7f;		//摩擦係数


	//カメラの剛体設定
	eShapeType type = eShapeType::eSphere;
	param.Reset();
	param.pos = camera.GetPosition();
	param.rot = camera.GetDirection();
	param.radius = radius;
	param.mass = mass;
	param.restitution = restitution;
	param.friction = friction;
	param.type = eMotionType::eActive;
	rigidID = GetGameAnimMgr()->CreateRigidBody(param, type);

	cameraOffset = - radius * 0.5f;	//気持ち中心より高くなるように

	//その他初期値設定
	metallic = 1.0f;
	roughness = 0.8f;

	pick = D3DXVECTOR3(0, 0, 0);
}




void PlayerObject::Update()
{

	UpdateMaterial();

	UpdatePick();

	UpdateCamera();

	UpdateLight();


}

//マテリアル更新
void PlayerObject::UpdateMaterial()
{
	//質感変更
	//ゴリ押し
	const float up = 0.02f;

	//ラフネス増減
	if (InputKeyManager()->GetKeyHold(VK_Z))
	{
		roughness = CLAMP(roughness - up, 0, 1);
	}
	if (InputKeyManager()->GetKeyHold(VK_X))
	{
		roughness = CLAMP(roughness + up, 0, 1);
	}

	//メタリック増減
	if (InputKeyManager()->GetKeyHold(VK_C))
	{
		metallic = CLAMP(metallic - up, 0, 1);
	}
	if (InputKeyManager()->GetKeyHold(VK_V))
	{
		metallic = CLAMP(metallic + up, 0, 1);
	}

	//マテリアルの総数
	int numMaterials = GetMaterialManager().GetNumMaterials();
	//デフォルトとゆーきたんのマテリアル変更
	auto mat = GetMaterialManager().GetMaterial(0);
	mat->metallic = metallic;
	mat->roughness = roughness;
	for (int i = 9; i < 21; ++i)
	{

		mat = GetMaterialManager().GetMaterial(i);
		mat->metallic = metallic;
		mat->roughness = roughness;
	}
}

//ライト更新
void PlayerObject::UpdateLight()
{
	//------------------------------------//
	//カメラの向いている方向に上から照らす//
	//--------------イメージ--------------//
	//                                    //
	//C:カメラ     |             L        //
	//L:ライト     |           ↙          //
	//T:ターゲット |                      //
	//             |     T     ←C        //
	//             ￣￣￣￣￣￣￣￣￣￣￣ //
	//------------------------------------//

	D3DXMATRIX lightMat;
	D3DXMatrixRotationQuaternion(&lightMat, &camera.GetDirection());
	D3DXVECTOR3 lightDir(0, -1, 0);
	lightDir += D3DXVECTOR3(lightMat._31, lightMat._32, lightMat._33);

	D3DXVec3Normalize(&lightDir, &lightDir);
	
	GetLightMamager().SetLightDirection(lightDir);

	GetLightMamager().UpdateLight();


}

void PlayerObject::UpdateCamera()
{

	const BYTE cameraLeftRot = VK_Q;
	const BYTE cameraRightRot = VK_E;
	const BYTE jump = VK_SPACE;

	D3DXVECTOR3 cameraPos = GetPhysicsMgr()->GetState(rigidID).GetPosition();
	cameraPos.y -= cameraOffset;
	D3DXQUATERNION cameraDir = camera.GetDirection();
	D3DXVECTOR3 moveVec = cameraPos - oldPos;

	D3DXVECTOR3 force(0, 0, 0);
	const float forceValue = 2.0f;
	const float rotAngle = 0.04f;

	//ワールド空間での力算出
	force.x = forceValue * InputKeyManager()->GetKeyVecX();
	force.z = forceValue * InputKeyManager()->GetKeyVecY();

	if (InputKeyManager()->GetKeyHold(VK_Q))
	{
		D3DXQUATERNION q;
		D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 1, 0), -rotAngle);
		cameraDir *= q;
	}
	if (InputKeyManager()->GetKeyHold(VK_E))
	{
		D3DXQUATERNION q;
		D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 1, 0), rotAngle);
		cameraDir *= q;
	}

	if (InputKeyManager()->GetKeyTrigger(VK_SPACE))
	{
		//カメラが一定以下の高さなら力を加える
		if (cameraPos.y <= 5.0f)
			force.y += 300;
	}

	//力を自身のローカル空間に持ってくる
	force = GameMath::Rotate(force, cameraDir);


	//紐付いている剛体に力を加える
	GetPhysicsMgr()->AddForce(rigidID, force);

	//カメラの揺れ
	//勢いがついているようにみえなくもないから入れてみる
	D3DXQUATERNION xRot;
	const float quakeCoeff =6.0f; //揺れの係数

	//移動ベクトルを自身のローカル空間に
	moveVec = GameMath::Rotate(moveVec, GameMath::Conj(cameraDir));

	D3DXQuaternionRotationAxis(&xRot, &D3DXVECTOR3(1, 0, 0), moveVec.z * quakeCoeff);

	camera.Quake(xRot);

	//カメラ更新
	camera.SetNextCamera(cameraDir, cameraPos, 1);
	camera.Update();

	oldPos = camera.GetPosition();
}

void PlayerObject::UpdatePick()
{

	//掴む処理
	if (InputMouseManager()->GetMouseTrigger(LEFT_TRIGGER))
	{
		//スクリーン座標をワールド座標に変換
		D3DXVECTOR2 mp;
		InputMouseManager()->GetAbsoluteMousePos(mp.x, mp.y);
		D3DXMATRIX vp = GetCBMatrixManager().GetViewMatrix() * GetCBMatrixManager().GetProjMatrix();// ConstantBufferManager::GetInstance()->GetViewMatrix() * ConstantBufferManager::GetInstance()->GetProjMatrix();
		D3DXMatrixInverse(&vp, nullptr, &vp);
		D3DXVECTOR3 nearPos(mp.x, mp.y, 0);
		D3DXVECTOR3 farPos(mp.x, mp.y, 1);
		nearPos = GameMath::GetWorldPos(nearPos, vp);
		farPos = GameMath::GetWorldPos(farPos, vp);

		//衝突していたら衝突点取得
		pick = GetGameAnimMgr()->PickStart(nearPos, farPos);
	}
	//掴んでいる最中の処理
	else if (InputMouseManager()->GetMouseHold(LEFT_HOLD))
	{
		D3DXVECTOR2 mp;
		InputMouseManager()->GetAbsoluteMousePos(mp.x, mp.y);
		D3DXMATRIX wvp = GetCBMatrixManager().GetViewMatrix() * GetCBMatrixManager().GetProjMatrix();

		D3DXVECTOR3 sp = GameMath::GetScreenPos(pick, wvp);
		D3DXVECTOR3 wp(mp.x, mp.y, sp.z);
		D3DXMatrixInverse(&wvp, nullptr, &wvp);
		wp = GameMath::GetWorldPos(wp, wvp);

		GetGameAnimMgr()->PickUpdate(wp);
	}
	//離した時の処理
	else if (InputMouseManager()->GetMouseRelease(LEFT_RELEASE))
	{
		GetGameAnimMgr()->PickEnd();
	}

}

