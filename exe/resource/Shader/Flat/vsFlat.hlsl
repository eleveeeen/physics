//2D描画用のシェーダ

struct VS_INPUT
{
	float3 pos		:POSITION;
	float2 texcoord	:TEXTURE;
};

struct VS_OUTPUT
{
	float4 pos		:SV_POSITION;
	float2 texcoord	:TEXTURE;

};

cbuffer worldMatrix:register(b0)
{
	matrix world;
}

cbuffer textureMatrix : register(b2)
{
	matrix tex;

}

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;

	output.pos = mul(float4(input.pos, 1), world);
	output.texcoord = mul(input.texcoord, tex);

	return output;
}