#include "Misc.h"
#include "GpuResourceUtils.h"
#include "SkydomeShader.h"

void SkydomeShader::SetRenderState(const RenderContext& rc)
{
	ID3D11DeviceContext* dc = rc.deviceContext;
	// �����_�[�X�e�[�g�ݒ�
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(rc.renderState->GetBlendState(BlendState::Opaque), blend_factor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(rc.renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(rc.renderState->GetRasterizerState(RasterizerState::SolidCullBack));
}

// �`��
void SkydomeShader::SetShaderResourceView(const ModelResource::Mesh& mesh, ID3D11DeviceContext*& dc)
{
	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	dc->PSSetShaderResources(0, 1, mesh.material->diffuseMap.GetAddressOf());
}