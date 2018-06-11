#include"psHeader.hlsli"
#include"../LambertModel/LambertModel.hlsli"
#include"../SpecularModel/SpecularModel.hlsli"
#include"../ReflectionModel/ReflectionModel.hlsli"

//�f�t�H���g�̃s�N�Z���V�F�[�_
//�����x�[�X�Ōv�Z

/*
�}�e���A���p�����[�^
float4 albedo 
float  roughness
float  metallic   

*/


iLambertModel lambertModel;
//float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir);

iSpecularModel specularModel;
//float4 GetColor(float3 normal, float3 posView, float3 lDir);

iReflectionModel reflectionModel;
//float4 GetColor(float3 posView, float3 normal);


float4 main(PS_INPUT input) : SV_TARGET
{
	//�A�e�v�Z�O�̃A���x�h�J���[
	float4 baseColor = albedo * albedoMap.Sample(Sampler, input.texcoord);


	//�A�e�v�Z
	float4 lambertColor = lambertModel.GetColor(baseColor, input.normal, input.posView, input.lightDir);
	lambertColor.xyz *= 1.0f - metallic;

	//�X�y�L�����v�Z
	float4 specularColor = specularModel.GetColor(input.normal, input.posView, input.lightDir);
	specularColor.xyz = (lerp(specularColor.xyz, baseColor.xyz, metallic));
	//IBL�ł̔���
	float4 reflectColor = reflectionModel.GetColor(input.posView, input.normal);
	reflectColor.xyz *= metallic;
	
	//debug
	// ���S�ɃR�����g�A�E�g����ƃR���p�C���G���[���������邩��ق�0�ɂ���
	//  lambertColor.xyz *= 0.0000001f;
	 //specularColor.xyz *= 0.000001f;
	// reflectColor.xyz *= 0.0000001f;
	 // return lambertColor + specularColor + reflectColor;
	return lambertColor + specularColor * reflectColor;	
}