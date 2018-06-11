//デフォルトのジオメトリシェーダ

#include"gsHeader.hlsli"

//面法線情報
interface iFaceNormal
{
	void Set(inout GS_INPUT input[3]);
};


class FaceFlat :iFaceNormal
{
	void Set(inout GS_INPUT input[3])
	{
		float3 normal = input[0].pos.xyz / input[0].pos.w;
			float3 v1 = (input[1].pos.xyz / input[1].pos.w) - normal;
			float3 v2 = (input[2].pos.xyz / input[2].pos.w) - normal;
		normal = normalize(cross(v1, v2));

		for (uint i = 0; i < 3; ++i)
		{
			input[i].normal = normal;
		}
	}
};

class FaceGouraud : iFaceNormal
{
	void Set(inout GS_INPUT input[3])
	{

	}
};


iFaceNormal faceNormal;

[maxvertexcount(6)]
void main(
	triangle GS_INPUT input[3] : SV_POSITION,
	inout TriangleStream< GS_OUTPUT > TriStream
)
{

	faceNormal.Set(input);

	GS_OUTPUT output;

	output.RTIndex = 0;
	output.lightDir = mul(lDirection.xyz, (float3x3)(view));
	for (uint i = 0; i < 3; ++i)
	{
		float4 posView = mul(input[i].pos, view);
		//ビュー変換
		output.posView = posView.xyz;
		//射影変換
		output.pos = mul(posView, pers);

		output.normal = mul(input[i].normal, view);

		output.texcoord = input[i].texcoord;

		TriStream.Append(output);
	}

	TriStream.RestartStrip();
	for (uint i = 0; i < 3; ++i)
	{
		float4 posView = mul(input[2-i].pos, view);
			//ビュー変換
			output.posView = posView.xyz;
		//射影変換
		output.pos = mul(posView, pers);

		output.normal = -mul(input[2-i].normal, view);

		output.texcoord = input[2-i].texcoord;

		TriStream.Append(output);
	}

}