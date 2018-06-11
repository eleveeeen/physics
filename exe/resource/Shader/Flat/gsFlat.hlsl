//2d描画用ジオメトリシェーダ

struct GS_OUTPUT
{
	float4 pos		:SV_POSITION;
	float2 texcoord	:TEXTURE;

};

struct GS_INPUT
{
	float4 pos		:SV_POSITION;
	float2 texcoord	:TEXTURE;

};

cbuffer proj:register(b1)
{
	matrix ortho;
	matrix pers;
}

[maxvertexcount(3)]
void main(
	triangle GS_INPUT input[3],
	inout TriangleStream< GS_OUTPUT > output
	)
{
	for (uint i = 0; i < 3; i++)
	{
		GS_OUTPUT element;
		element.pos = mul(input[i].pos, ortho);
		element.texcoord = input[i].texcoord;

		output.Append(element);
	}

}