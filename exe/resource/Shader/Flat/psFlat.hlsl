struct PS_INPUT
{
	float4 pos		:SV_POSITION;
	float2 texcoord	:TEXTURE;

};

//�e�N�X�`��
Texture2D diffuseTexture:register(t5);
//�T���v��
SamplerState Sampler:register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	return diffuseTexture.Sample(Sampler, input.texcoord);
}