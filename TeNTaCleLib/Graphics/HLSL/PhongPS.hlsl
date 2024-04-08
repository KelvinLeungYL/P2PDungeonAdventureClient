#include "Phong.hlsli"

Texture2D diffuseMap : register(t0);
SamplerState linearSampler : register(s0);
Texture2D normalMap : register(t1);

float4 main(VS_OUT pin) : SV_TARGET
{
    float4 diffuseColor = diffuseMap.Sample(linearSampler, pin.texcoord) * pin.color;
    if (linearGamma != 1.0f && linearGamma > 0.0f) diffuseColor.rgb = pow(diffuseColor, 1.0f / linearGamma);
    return diffuseColor;
	
    float3 normal = normalize(2 * normalMap.Sample(linearSampler, pin.texcoord).xyz - 1);
    float3x3 CM = float3x3(normalize(pin.tangent), normalize(pin.binormal), normalize(pin.normal));
    float3 N = normalize(mul(normal, CM));

    float3 L = normalize(directionalLightData.direction.xyz);
    float3 E = normalize(cameraPosition.xyz - pin.position.xyz);
    
	// �}�e���A���萔
    float3 ka = float3(1, 1, 1);
    float3 kd = float3(1, 1, 1);
    float3 ks = float3(1, 1, 1);
    float shiness = 128;
   
    // �����̌v�Z
    float3 ambient = ka * ambientLightColor.xyz;
    
    float3 directionalDiffuse = ClacHalfLambert(N, L, directionalLightData.color.rgb, kd);
    float3 directionalSpecular = CalcPhongSpecular(N, L, directionalLightData.color.rgb, E, shiness, ks);
    
    // �_�����̏���
	float3 pointDiffuse = (float3)0;
	float3 pointSpecular = (float3)0;
	int i;
	for (i = 0; i < pointLightCount; ++i)
	{
		// ���C�g�x�N�g�����Z�o
        float3 lightVector = pin.position.xyz - pointLightData[i].position.xyz;

		// ���C�g�x�N�g���̒������Z�o
		float lightLength = length(lightVector);

		// ���C�g�̉e���͈͊O�Ȃ��̌v�Z�����Ȃ��B
		if (lightLength > pointLightData[i].range) continue;

		// �����������Z�o����
		float attenuate = clamp(1.0f - lightLength / pointLightData[i].range, 0.0, 1.0);
		lightVector = lightVector / lightLength;
		pointDiffuse += CalcLambertDiffuse(N, lightVector, pointLightData[i].color.rgb, kd.rgb) * attenuate;
		pointSpecular += CalcPhongSpecular(N, lightVector, pointLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
	}

	// �X�|�b�g���C�g�̏���
	float3 spotDiffuse = (float3)0;
	float3 spotSpecular = (float3)0;
	for (i = 0; i < spotLightCount; ++i)
	{
		// ���C�g�x�N�g�����Z�o
        float3 lightVector = pin.position.xyz - spotLightData[i].position.xyz;

		// ���C�g�x�N�g���̒������Z�o
		float lightLength = length(lightVector);

		if (lightLength > spotLightData[i].range) continue;

		// �����������Z�o����
		float attenuate = clamp(1.0f - lightLength / spotLightData[i].range, 0.0, 1.0);

		lightVector = normalize(lightVector);

		// �p�x�������Z�o����attenuate�ɏ�Z����
		float3 spotDirection = spotLightData[i].direction;;
		float angle = dot(lightVector, spotDirection);
		float area = spotLightData[i].innerCorn - spotLightData[i].outerCorn;
		attenuate *= clamp(1.0f - (spotLightData[i].innerCorn - angle) / area, 0.0, 1.0);

		spotDiffuse += CalcLambertDiffuse(N, lightVector, spotLightData[i].color.rgb, kd.rgb) * attenuate;
		spotSpecular += CalcPhongSpecular(N, lightVector, spotLightData[i].color.rgb, E, shiness, ks.rgb) * attenuate;
	}
    

    float4 color = float4(ambient, diffuseColor.a);
    color.rgb += diffuseColor.rgb * (directionalDiffuse + pointDiffuse + spotDiffuse);
    color.rgb += directionalSpecular + pointSpecular + spotSpecular;

	//	�������C�e�B���O
    color.rgb += CalcRimLight(N, E, L, directionalLightData.color.rgb);

    
	
    return color;
}