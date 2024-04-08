#include	"Light.hlsli"

struct VS_OUT
{
    float4 vertex   : SV_POSITION;
    float2 texcoord : TEXCOORD;
    float3 normal   : NORMAL;
    float3 position : POSITION;
    float4 color    : COLOR;
};

cbuffer CbScene : register(b0)
{
    float4 cameraPosition;
    row_major float4x4 viewProjection;
    float4 ambientLightColor;
    DirectionalLightData directionalLightData;
    PointLightData pointLightData[PointLightMax];   // �_�������
    SpotLightData spotLightData[SpotLightMax];      // �X�|�b�g���C�g���
    int pointLightCount;                            // �_������
    int spotLightCount;                             // �X�|�b�g���C�g��
    float2 dummy2;
}

cbuffer CbMesh : register(b1)
{
    float4 materialColor;
    float linearGamma;
}