#include "LuminanceExtractionShader.h"
#include "GpuResourceUtils.h"

LuminanceExtractionShader::LuminanceExtractionShader(ID3D11Device* device) : SpriteShader(device, "Data/Shader/SpriteVS.cso", "Data/Shader/LuminanceExtractionPS.cso")
{
	// �V�[���p�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CBLuminanceExtraction),
		constantBuffer.GetAddressOf()
	);
}

void LuminanceExtractionShader::UpdateConstantBuffer(const RenderContext& rc)
{
	CBLuminanceExtraction cbLuminanceExtraction;
	cbLuminanceExtraction.threshold = rc.luminanceExtractionData.threshold;
	cbLuminanceExtraction.intensity = rc.luminanceExtractionData.intensity;
	rc.deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbLuminanceExtraction, 0, 0);
}