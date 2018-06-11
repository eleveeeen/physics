//ジオメトリシェーダ用のヘッダー


struct GS_INPUT
{
	float4 pos		:SV_POSITION;
	float3 normal	:NORMAL;
	float2 texcoord	:TEXTURE;
};

struct GS_OUTPUT
{
	float4 pos		:SV_POSITION;
	float3 normal	:NORMAL;
	float2 texcoord	:TEXTURE;
	float3 posView	:POSITION_VIEW;
	float3 lightDir :LIGHTDIRECTION;
	uint RTIndex	:SV_RenderTargetArrayIndex;
};


cbuffer ViewMatrix : register(b0)
{
	matrix view;
}

cbuffer ProjMatrix : register(b1)
{
	matrix ortho;
	matrix pers;
}

cbuffer CubeMap : register(b2)
{
	matrix cubeProj;
	matrix cubeView[6];
	float4 cubeCamera;
}

cbuffer Light : register(b3)
{
	float4 lDiffuse;
	float4 lAmbient;
	float4 lDirection;
}