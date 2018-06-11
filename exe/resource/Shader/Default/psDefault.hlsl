#include"psHeader.hlsli"
#include"../LambertModel/LambertModel.hlsli"
#include"../SpecularModel/SpecularModel.hlsli"
#include"../ReflectionModel/ReflectionModel.hlsli"

//デフォルトのピクセルシェーダ
//物理ベースで計算

/*
マテリアルパラメータ
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
	//陰影計算前のアルベドカラー
	float4 baseColor = albedo * albedoMap.Sample(Sampler, input.texcoord);


	//陰影計算
	float4 lambertColor = lambertModel.GetColor(baseColor, input.normal, input.posView, input.lightDir);
	lambertColor.xyz *= 1.0f - metallic;

	//スペキュラ計算
	float4 specularColor = specularModel.GetColor(input.normal, input.posView, input.lightDir);
	specularColor.xyz = (lerp(specularColor.xyz, baseColor.xyz, metallic));
	//IBLでの反射
	float4 reflectColor = reflectionModel.GetColor(input.posView, input.normal);
	reflectColor.xyz *= metallic;
	
	//debug
	// 完全にコメントアウトするとコンパイルエラーが発生するからほぼ0にする
	//  lambertColor.xyz *= 0.0000001f;
	 //specularColor.xyz *= 0.000001f;
	// reflectColor.xyz *= 0.0000001f;
	 // return lambertColor + specularColor + reflectColor;
	return lambertColor + specularColor * reflectColor;	
}