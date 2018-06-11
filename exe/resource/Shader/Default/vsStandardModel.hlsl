//デフォルトのアニメーション無しモデル用
#include"vsHeader.hlsli"

VS_OUTPUT main(VS_STANDARDINPUT input)
{
	VS_OUTPUT output;
	
	//ワールド変換
	output.pos = mul(float4(input.pos, 1), world);
	output.normal = mul(input.normal, world);

	output.texcoord = input.texcoord;

	//output.eye = normalize(cameraPos.xyz - output.pos.xyz);

	return output;
}