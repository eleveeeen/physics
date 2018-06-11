cbuffer WorldMatrix :register(b0)
{
	matrix world;
}


cbuffer BoneMatrix:register(b1)
{
	matrix bone[256];
}

cbuffer TextureMatrix : register(b2)
{
	matrix texMat;
}

cbuffer CameraPos : register(b3)
{
	float4 cameraPos;
}

struct VS_STANDARDINPUT
{
	float3 pos			:POSITION;
	float3 normal		:NORMAL;
	float2 texcoord		:TEXTURE;
};

struct VS_SKINNEDINPUT
{
	float3 pos			:POSITION;
	float3 normal		:NORMAL;
	float2 texcoord		:TEXTURE;
	float4 blendWeight	:BLENDWEIGHT;
	uint4 blendIndex	:BLENDINDICES;
};

struct VS_OUTPUT
{
	float4 pos		:SV_POSITION;
	float3 normal	:NORMAL;
	float2 texcoord	:TEXTURE;
};