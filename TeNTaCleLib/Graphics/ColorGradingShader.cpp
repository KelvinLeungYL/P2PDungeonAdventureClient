#include "ColorGradingShader.h"
#include "GpuResourceUtils.h"

ColorGradingShader::ColorGradingShader(ID3D11Device* device) : SpriteShader(device, "Data/Shader/SpriteVS.cso", "Data/Shader/ColorGradingPS.cso")
{
	// �V�[���p�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CBColorGrading),
		constantBuffer.GetAddressOf()
	);
}

void ColorGradingShader::UpdateConstantBuffer(const RenderContext& rc)
{
	CBColorGrading cbColorGrading;
	cbColorGrading.hueShift = rc.colorGradingData.hueShift;
	cbColorGrading.saturation = rc.colorGradingData.saturation;
	cbColorGrading.brightness = rc.colorGradingData.brightness;
	rc.deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbColorGrading, 0, 0);
}