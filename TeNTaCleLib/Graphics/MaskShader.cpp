#include "MaskShader.h"
#include "GpuResourceUtils.h"

MaskShader::MaskShader(ID3D11Device* device) : SpriteShader(device, "Data/Shader/SpriteVS.cso", "Data/Shader/MaskPS.cso")
{
	// �V�[���p�o�b�t�@
	GpuResourceUtils::CreateConstantBuffer(
		device,
		sizeof(CbMask),
		constantBuffer.GetAddressOf()
	);
}

void MaskShader::UpdateConstantBuffer(const RenderContext& rc)
{
	//	�萔�o�b�t�@�̒��g���X�V���܂�
	CbMask cbMask;
	cbMask.dissolveThreshold = rc.maskData.dissolveThreshold;
	cbMask.edgeThreshold = rc.maskData.edgeThreshold;
	cbMask.edgeColor = rc.maskData.edgeColor;
	rc.deviceContext->UpdateSubresource(constantBuffer.Get(), 0, 0, &cbMask, 0, 0);
}