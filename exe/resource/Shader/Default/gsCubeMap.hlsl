#include"gsheader.hlsli"
//�L���[�u�}�b�v�p�W�I���g���V�F�[�_

[maxvertexcount(18)]
void main(
	triangle GS_INPUT input[3], 
	inout TriangleStream< GS_OUTPUT > TriStream
)
{

	for (uint i = 0; i < 6; i++)
	{
		GS_OUTPUT output;
		output.RTIndex = i;

		float3 vL = mul(lDirection.xyz, (float3x3) cubeView[i]);
		//�r���[��Ԃł̃��C�g�̔��˃x�N�g���Z�o
		output.lightDir = reflect(vL, float3(0, 0, -1));



		for (uint j = 0; j < 3; j++)
		{
			float4 posView = mul(input[j].pos, cubeView[i]);
			//�r���[�ϊ�
			output.posView = posView.xyz;
			//�ˉe�ϊ�
			output.pos = mul(posView, pers);

			output.normal = mul(input[j].normal, cubeView[i]);
			output.texcoord = input[j].texcoord;


			//output.eye  = mul((cubeCamera.xyz - input[j].pos.xyz),cubeView[i]);
		
			TriStream.Append(output);

		}

		TriStream.RestartStrip();

	}
}