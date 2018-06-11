//ピクセルシェーダ用のヘッダー


struct PS_INPUT
{
	float4 pos		:SV_POSITION;
	float3 normal	:NORMAL;
	float2 texcoord	:TEXTURE;
	float3 posView	:POSITION_VIEW;
	float3 lightDir :LIGHTDIRECTION;

};

cbuffer Material : register(b0)
{
	float4 albedo;
	float roughness;
	float metallic;
}

cbuffer Light : register(b1)
{
	float4 diffuse;
	float4 ambient;
	float4 direction;
}

cbuffer CubeParallax : register(b2)
{
	matrix invView;
	matrix cubeLocal;
	float4 cubeCenter;
}

//テクスチャ
Texture2D albedoMap:register(t0);
Texture2D specularMap:register(t1);
Texture2D normalMap:register(t2);
Texture2D sphereMap:register(t3);
TextureCube cubeMap:register(t4);
Texture2D msaaMap:register(t5);

//サンプラー
SamplerState Sampler:register(s0);

