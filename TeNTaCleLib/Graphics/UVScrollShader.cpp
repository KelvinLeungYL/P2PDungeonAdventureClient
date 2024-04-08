#include "UVScrollShader.h"
#include "GpuResourceUtils.h"

UVScrollShader::UVScrollShader(ID3D11Device* device) : SpriteShader(device, "Data/Shader/SpriteVS.cso", "Data/Shader/UVScrollPS.cso")
{
	// �V�[���p�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CbScroll),
		constantBuffer.GetAddressOf()
	);
}

void UVScrollShader::UpdateConstantBuffer(const RenderContext& rc)
{
	CbScroll cbScroll;
	cbScroll.uvScrollValue = rc.uvScrollData.uvScrollValue;
	rc.deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbScroll, 0, 0);
}