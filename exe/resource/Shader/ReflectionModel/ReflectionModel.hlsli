//反射モデル計算用のインターフェースとクラスヘッダー

interface iReflectionModel
{
	float4 GetColor(float3 posView, float3 normal);
};


//反射無し
class NoneReflection : iReflectionModel
{
	float4 GetColor(float3 posView, float3 normal)
	{
		return float4(0, 0, 0, 0);
	}
};


//キューブマップ
class CubeMap : iReflectionModel
{

	float4 GetColor(float3 posView, float3 normal)
	{

		float3 posWorld = mul(float4(posView, 1.0f), invView);
			float3 posLocal = mul(float4(posWorld, 1.0f), cubeLocal);

			float3 worldView = mul(normalize(posView), (float3x3)invView);
			float3 worldNormal = mul(normalize(normal), (float3x3)invView);

			//反射ベクトル算出
			float3 refWorld = reflect(worldView,worldNormal);// mul(reflect(-eye, normal), (float3x3)invView);
			float3 refLocal = mul(refWorld, (float3x3)cubeLocal);

			float3 cameraWorld = mul(float4(0.0f, 0.0f, 0.0f, 1.0f), invView);
			float3 cameraLocal = mul(float4(cameraWorld, 1.0f), cubeLocal);
		
		//衝突点までの距離算出
		float3 unitary = float3(1,1,1);
		float3 firstPlane = (-unitary + posLocal) / refLocal;
		float3 secondPlane = (unitary + posLocal) / refLocal;
		float3 furthestPlane = abs(max(firstPlane, secondPlane));
		float distance = min(furthestPlane.x, min(furthestPlane.y, furthestPlane.z));

		float3 intersectPos = posWorld + refWorld * distance;
		float3 refDir = intersectPos - cubeCenter.xyz;
	
		return float4(cubeMap.SampleLevel(Sampler, normalize(refDir), (roughness)* 8).xyz, 0);
	}

};

//スフィアマップ
class SphereMap : iReflectionModel
{
	float4 GetColor(float3 posView, float3 normal)
	{

		float3 E = -normalize(posView);
		float3 N = normalize(normal);
		float3 R = E - 2 * dot(E, N)*N;
		float m = 2 * sqrt(R.x*R.x + R.y*R.y + (R.z + 1)*(R.z + 1));
		float2 uv;
		uv.x = R.x / m + 0.5;
		uv.y = R.y / m + 0.5;
	
		return float4(sphereMap.SampleLevel(Sampler, uv, (roughness)* 8).xyz, 0);
	}
};

/*
簡易スフィア環境マップメモ
uv.x = normal.x*0.5 + 0.5;
uv.y = normal.y*0.5 + 0.5;
*/