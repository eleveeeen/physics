//スペキュラ計算用のインターフェースとクラスヘッダー

interface iSpecularModel
{
	float4 GetColor(float3 normal, float3 posView, float3 lDir);
};

//GGX
//metallicが0の時は処理無し
class GGX : iSpecularModel
{
	float4 GetColor(float3 normal, float3 posView, float3 lDir)
	{

		float3 L = -normalize(lDir);	//ライトベクトル
		float3 E = -normalize(posView);		//視線ベクトル
		float3 N = normalize(normal);	//法線ベクトル
		float3 H = normalize(L + E);	//ハーフベクトル

		float r2 = roughness * roughness;

		float NL = (dot(N, L));
		float NE = (dot(N, E));
		float NH = (dot(N, H));
		float LH = (dot(L, H));

		//D
		const float pi = 3.14159f;
		float r4 = r2 * r2;
		float denom = NH * NH * (r4 - 1.0f) + 1.0f;
		float D = r4 / (pi * denom * denom);

		//F
		float LH5 = pow(1.0f - LH, 5);
		float F = lerp(LH5, metallic, metallic);

		//V
		float k = r2 / 2.0f;
		float nk = 1.0f - k;
		float V = (1.0f / ((NL * nk) + k) * (1.0f / ((NE * nk) + k)));

		float spe = saturate(NL * D * F * V);

		return float4(spe, spe, spe, 0);
	}
};

//処理無し
class ZeroSpecular : iSpecularModel
{
	float4 GetColor(float3 normal, float3 posView, float3 lDir)
	{
		return float4(0,0,0,0);
	}
};
