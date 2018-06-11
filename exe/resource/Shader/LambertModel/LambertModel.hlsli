//ランバートモデルのインターフェースとクラスヘッダー

interface iLambertModel
{
	float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir);
};


//ライティング無し
class NoneLighting : iLambertModel
{
	float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir)
	{

		return baseColor;
	}

};

//ランバート
class Lambert : iLambertModel
{
	float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir)
	{
		float3 L = -normalize(lDir);

		float3 N = normalize(normal);

		float NL = dot(N, L);

		return float4(baseColor.xyz * NL, baseColor.a);
	}
};


//オーレンネイヤー
//ラフネスが0の時はA+B*C*Dの結果が1で
//(baseColor * dot(L,N) * 1)になるからランバートで処理
class OrenNayar : iLambertModel
{
	float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir)
	{
	
		float3 L = -normalize(lDir);	//ライトベクトル

		float3 N = normalize(normal);	//法線ベクトル

		float3 E = -normalize(posView);		//視線ベクトル

		float LE = dot(L, E);
		float NL = dot(N, L);
		float NE = dot(N, E);

		float r2 = roughness * roughness;


		// 1 - 0.5(roughness^2 / (roughness^2 + 0.33))
		float A = 1 - 0.5f * (r2 / (r2 + 0.33f));

		// 0.45(roughness^2 / (roughness^2 + 0.09))
		float B = 0.45f * (r2 / (r2 + 0.09f));


		//max(0,(normalize(L - (L・N)N)・normalize(E - (E・N)N))
		float3 al = normalize(L - NL * N);
		float3 ae = normalize(E - NE * N);
		float C = max(0, dot(al, ae));

		//sinα * tanβ
		//α = max(acos(N・L),acos(N・E))
		//β = min(acos(N・L),acos(N・E))
		//ここは事前計算でよさそう

		float thetaI = acos(NL);
		float thetaR = acos(NE);
		float a = max(thetaI, thetaR);
		float b = min(thetaI, thetaR);
		float D = sin(a) * tan(b);

		return float4(baseColor.xyz * NL * (A + B * C * D), baseColor.a);
	}
};