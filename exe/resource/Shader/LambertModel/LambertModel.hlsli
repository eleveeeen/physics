//�����o�[�g���f���̃C���^�[�t�F�[�X�ƃN���X�w�b�_�[

interface iLambertModel
{
	float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir);
};


//���C�e�B���O����
class NoneLighting : iLambertModel
{
	float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir)
	{

		return baseColor;
	}

};

//�����o�[�g
class Lambert : iLambertModel
{
	float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir)
	{
		float3 L = -normalize(lDir);

		float3 N = normalize(normal);

		float NL = dot(N, L);

		return float4(baseColor.xyz * NL, baseColor.a);
	}
};


//�I�[�����l�C���[
//���t�l�X��0�̎���A+B*C*D�̌��ʂ�1��
//(baseColor * dot(L,N) * 1)�ɂȂ邩�烉���o�[�g�ŏ���
class OrenNayar : iLambertModel
{
	float4 GetColor(float4 baseColor, float3 normal, float3 posView, float3 lDir)
	{
	
		float3 L = -normalize(lDir);	//���C�g�x�N�g��

		float3 N = normalize(normal);	//�@���x�N�g��

		float3 E = -normalize(posView);		//�����x�N�g��

		float LE = dot(L, E);
		float NL = dot(N, L);
		float NE = dot(N, E);

		float r2 = roughness * roughness;


		// 1 - 0.5(roughness^2 / (roughness^2 + 0.33))
		float A = 1 - 0.5f * (r2 / (r2 + 0.33f));

		// 0.45(roughness^2 / (roughness^2 + 0.09))
		float B = 0.45f * (r2 / (r2 + 0.09f));


		//max(0,(normalize(L - (L�EN)N)�Enormalize(E - (E�EN)N))
		float3 al = normalize(L - NL * N);
		float3 ae = normalize(E - NE * N);
		float C = max(0, dot(al, ae));

		//sin�� * tan��
		//�� = max(acos(N�EL),acos(N�EE))
		//�� = min(acos(N�EL),acos(N�EE))
		//�����͎��O�v�Z�ł悳����

		float thetaI = acos(NL);
		float thetaR = acos(NE);
		float a = max(thetaI, thetaR);
		float b = min(thetaI, thetaR);
		float D = sin(a) * tan(b);

		return float4(baseColor.xyz * NL * (A + B * C * D), baseColor.a);
	}
};