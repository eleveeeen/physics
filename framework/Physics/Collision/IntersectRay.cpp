#include"IntersectRay.h"

#include"shape.h"
#include"RayFunc.h"
#include"../../Utility/GameMath.h"


//ボックス
bool IntersectRayBox(const RayInput& ray, RayOutput& out, const Box& box, const D3DXMATRIX& transform)
{
	//レイをボックスのローカル座標へ変換

	D3DXMATRIX transformBox = GameMath::OrthoInverse3x4(transform);
	D3DXVECTOR3 rayStartPos = GameMath::Rotate(ray.startPos, GameMath::Get3x3(transformBox))
		+ GameMath::GetTranslation(transformBox);

	D3DXVECTOR3 rayDirection = GameMath::Rotate(ray.dir, GameMath::Get3x3(transformBox));

	//交差判定
	float tmpVariable = 0.0f;
	D3DXVECTOR3 tmpNormal(0.0f, 0.0f, 0.0f);

	if (IntersectRayAABB(rayStartPos, rayDirection, GameMath::GetVec3(0.0f), box.half, tmpVariable, tmpNormal))
	{
		//衝突点が 0～今格納している衝突点の間にあれば更新
		if (tmpVariable > 0.0f && tmpVariable < out.variable)
		{
			out.contactFlag = true;
			out.variable = tmpVariable;
			out.contactPoint = ray.startPos + tmpVariable*ray.dir;
			out.contactNormal = GameMath::Rotate(tmpNormal, GameMath::Get3x3(transform));
			out.subData.type = SubData::NONE;
			return true;

		}


	}

	return false;

}


//スフィア
bool IntersectRaySphere(const RayInput& ray, RayOutput& out, const Sphere& sphere, const D3DXMATRIX& transform)
{


	D3DXVECTOR3 v = ray.startPos - GameMath::GetTranslation(transform);

	float a = D3DXVec3Dot(&ray.dir, &ray.dir);

	float b = D3DXVec3Dot(&v, &ray.dir);

	float c = D3DXVec3Dot(&v, &v) - sphere.radius*sphere.radius;

	if (c < 0.0f)
		return false;


	float d = b*b - a*c;

	if (d < 0.0f || GameMath::AbsF(a) < 0.00001f)
		return false;

	float tt = (-b - sqrtf(d)) / a;

	//範囲外か判定
	if (tt<0.0f || tt>1.0f)
		return false;


	//選択しているオブジェより近くにいるか判定
	if (tt < out.variable)
	{
		out.contactFlag = true;
		out.variable = tt;
		out.contactPoint = ray.startPos + tt*ray.dir;
		out.contactNormal = GameMath::Normalize(out.contactPoint - GameMath::GetTranslation(transform));
		out.subData.type = SubData::NONE;
		return true;

	}

	return false;
}


//カプセル
bool IntersectRayCapsule(const RayInput& ray, RayOutput& out, const Capsule& capsule, const D3DXMATRIX& transform)
{

	//レイをカプセルのローカルへ変換

	D3DXMATRIX transformCapsule = GameMath::OrthoInverse3x4(transform);

	D3DXVECTOR3 startPosL = GameMath::Rotate(ray.startPos, (transformCapsule))
		+ GameMath::GetTranslation(transformCapsule);

	D3DXVECTOR3 rayDirL = GameMath::Rotate(ray.dir, (transformCapsule));

	float radSqr = capsule.radius * capsule.radius;

	//始点とカプセルの内外判定

	{
		float h = startPosL.x;
		//-half～halfの間にクランプ
		h = CLAMP(h, -capsule.halfLength, capsule.halfLength);
		D3DXVECTOR3 px(h, 0.0f, 0.0f);
		float sqrLen = D3DXVec3LengthSq(&(startPosL - px));
		if (sqrLen <= radSqr)
			return false;
	}

	

	//カプセルの胴体との交差判定
	
	do
	{
		D3DXVECTOR3 pos(startPosL);
		D3DXVECTOR3 dir(rayDirL);
		pos.x = 0.0f;
		dir.x = 0.0f;


		float a = D3DXVec3Dot(&dir, &dir);

		float b = D3DXVec3Dot(&pos, &dir);

		float c = D3DXVec3Dot(&pos, &pos) - radSqr;

		float d = b*b - a*c;

		//レイが逸れているから終了
		if (d < 0.0f)
			return false;

		//レイがx軸と平行だったら処理を抜ける
		if (GameMath::AbsF(a) < 0.00001f)
			break;


		float tt = (-b - sqrtf(d)) / a;

		//少なくとも胴体とはあたっていない
		if (tt<0.0f || tt>1.0f)
			break;

		if (tt < out.variable)
		{
			D3DXVECTOR3 cp = startPosL + rayDirL*tt;

			//胴体と衝突 
			if (GameMath::AbsF(cp.x) <= capsule.halfLength)
			{
				out.contactFlag = true;
				out.variable = tt;

				out.contactPoint = GameMath::Transform(cp, transform);
				out.contactNormal = GameMath::Rotate(GameMath::Normalize(cp), GameMath::Get3x3(transform));
				out.subData.type = SubData::NONE;
				return true;
			}
		}


	} while (0);
	
	//カプセルの球体部分と交差判定

	float a = D3DXVec3Dot(&rayDirL, &rayDirL);

	if (GameMath::AbsF(a) < 0.00001f)
		return false;


	do
	{
		D3DXVECTOR3 center(capsule.halfLength, 0.0f, 0.0f);
		D3DXVECTOR3 v = startPosL - center;

		float b = D3DXVec3Dot(&v, &rayDirL);

		float c = D3DXVec3Dot(&v, &v) - radSqr;

		float d = b*b - a*c;

		if (d < 0.0f)
			break;

		float tt = (-b - sqrtf(d)) / a;

		if (tt<0.0f || tt>1.0f)
			break;

		if (tt < out.variable)
		{
			D3DXVECTOR3 cp = startPosL + rayDirL*tt;
			out.contactFlag = true;
			out.variable = tt;
			out.contactPoint = ray.startPos + ray.dir*tt;
			out.contactNormal = GameMath::Rotate(GameMath::Normalize(cp - center), transform);
			out.subData.type = SubData::NONE;
			return true;
		}
	} while (0);

	{
		D3DXVECTOR3 center(-capsule.halfLength, 0.0f, 0.0f);

		D3DXVECTOR3 v = startPosL - center;
		float b = D3DXVec3Dot(&v, &rayDirL);

		float c = D3DXVec3Dot(&v, &v) - radSqr;

		float d = b*b - a*c;

		if (d < 0.0f)
			return false;

		float tt = (-b - sqrtf(d)) / a;

		if (tt<0.0f || tt>1.0f)
			return false;

		if (tt < out.variable)
		{

			D3DXVECTOR3 cp = startPosL + rayDirL*out.variable;
			out.contactFlag = true;
			out.variable = tt;
			out.contactPoint = ray.startPos + ray.dir*tt;
			out.contactNormal = GameMath::Rotate(GameMath::Normalize(cp - center), transform);
			out.subData.type = SubData::NONE;
			return true;
		}
	}
	return false;
}



//シリンダー
bool IntersectRayCylinder(const RayInput& ray, RayOutput& out, const Cylinder& cylinder, const D3DXMATRIX& transform)
{

	//レイをシリンダーのローカルに変換
	D3DXMATRIX transformCapsule = GameMath::OrthoInverse3x4(transform);

	D3DXVECTOR3 startPosL = GameMath::Rotate(ray.startPos, transformCapsule) + GameMath::GetTranslation(transformCapsule);

	D3DXVECTOR3 rayDirL = GameMath::Rotate(ray.dir, transformCapsule);

	float radSqr = cylinder.radius*cylinder.radius;


	//始点が円柱の中にあるか判定
	{
		float h = startPosL.x;
		if (-cylinder.halfLength <= h && h <= cylinder.halfLength)
		{
			D3DXVECTOR3 px(h, 0, 0);
			float sqrLen = D3DXVec3LengthSq(&(startPosL - px));
			if (sqrLen <= radSqr)
				return false;
		}

	}



	//胴体との内外判定
	do
	{

		D3DXVECTOR3 pos(startPosL);
		D3DXVECTOR3 dir(rayDirL);

		pos.x = 0;
		dir.x = 0;

		float a = D3DXVec3Dot(&dir, &dir);

		float b = D3DXVec3Dot(&pos, &dir);

		float c = D3DXVec3Dot(&pos, &pos) - radSqr;

		float d = b*b - a*c;

		if (d < 0.0f)
			return false;

		if (GameMath::AbsF(a) < 0.00001f)
			break;

		float tt = (-b - sqrtf(d)) / a;

		if (tt<0.0f || tt>1.0f)
			break;


		if (tt < out.variable)
		{

			D3DXVECTOR3 cp = startPosL + rayDirL*tt;
			if (GameMath::AbsF(cp.x) <= cylinder.halfLength)
			{
				out.contactFlag = true;
				out.variable = tt;
				out.contactPoint = GameMath::Transform(cp, transform);
				out.contactNormal = GameMath::Rotate(GameMath::Normalize(cp), transform);
				out.subData.type = SubData::NONE;
				return true;
			}
		}
	} while (0);


	//円柱の端にある平面の円との判定
	{


		if (GameMath::AbsF(rayDirL.x) < 0.00001f)
			return false;

		float t1 = (cylinder.halfLength - startPosL.x) / rayDirL.x;
		float t2 = (-cylinder.halfLength - startPosL.x) / rayDirL.x;

		float tt = min(t1, t2);

		if (tt<0.0f || tt>1.0f)
			return false;

		D3DXVECTOR3 pos = startPosL + rayDirL*tt;
		pos.x = 0.0f;

		if (D3DXVec3LengthSq(&pos) < radSqr && tt < out.variable)
		{
			D3DXVECTOR3 cp = startPosL + rayDirL*tt;
			out.contactFlag = true;
			out.variable = tt;
			out.contactPoint = ray.startPos + ray.dir*tt;
			if (cp.x > 0.f)
			{
				out.contactNormal = GameMath::Rotate(D3DXVECTOR3(1.0f, 0.0f, 0.0f), transform);
			}
			else
			{
				out.contactNormal = GameMath::Rotate(D3DXVECTOR3(-1.0f, 0.0f, 0.0f), transform);
			}

			out.subData.type = SubData::NONE;
			return true;
		}
	}

	return false;
}









