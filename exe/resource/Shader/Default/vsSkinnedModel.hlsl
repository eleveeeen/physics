//�f�t�H���g�̃A�j���[�V�����������f���p
#include"vsHeader.hlsli"

VS_OUTPUT main(VS_SKINNEDINPUT input)
{
	VS_OUTPUT output;

	//���_�u�����f�B���O
	matrix comb = (matrix)0;
	comb = bone[input.blendIndex.x] * input.blendWeight.x;
	comb += bone[input.blendIndex.y] * input.blendWeight.y;
	comb += bone[input.blendIndex.z] * input.blendWeight.z;
	comb += bone[input.blendIndex.w] * input.blendWeight.w;


	float weight = input.blendWeight.x + input.blendWeight.y + input.blendWeight.z + input.blendWeight.w;
	comb /= weight;

	output.pos =  mul(float4(input.pos,1), comb);
	output.normal = mul(input.normal, (float3x3)comb);

	//���[���h�ϊ�
	output.pos = mul(output.pos, world);
	output.normal = mul(output.normal, world);



	output.texcoord = input.texcoord;

	//output.eye = normalize(cameraPos.xyz - output.pos.xyz);

	return output;
}