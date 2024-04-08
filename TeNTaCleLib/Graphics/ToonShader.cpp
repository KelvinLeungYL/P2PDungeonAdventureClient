#include "Misc.h"
#include "GpuResourceUtils.h"
#include "ToonShader.h"

ToonShader::ToonShader(ID3D11Device* device) : ModelShader(device, "Data/Shader/ToonVS.cso", "Data/Shader/ToonPS.cso")
{
	HRESULT hr = GpuResourceUtils::LoadTexture(device, "Data/Sprites/ramp3.png", toontexture.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
}

void ToonShader::SetRenderState(const RenderContext& rc)
{
	ID3D11DeviceContext* dc = rc.deviceContext;
	// �����_�[�X�e�[�g�ݒ�
	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	dc->OMSetBlendState(rc.renderState->GetBlendState(BlendState::OIT), blend_factor, 0xFFFFFFFF);
	dc->OMSetDepthStencilState(rc.renderState->GetDepthStencilState(DepthState::TestAndWrite), 0);
	dc->RSSetState(rc.renderState->GetRasterizerState(RasterizerState::SolidCullNone));
}

// �`��
void ToonShader::SetShaderResourceView(const ModelResource::Mesh& mesh, ID3D11DeviceContext*& dc)
{
	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	ID3D11ShaderResourceView* srvs[] =
	{
		mesh.material->diffuseMap.Get(),
		toontexture.Get(),
	};
	dc->PSSetShaderResources(0, _countof(srvs), srvs);
}