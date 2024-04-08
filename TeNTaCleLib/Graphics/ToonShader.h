#pragma once

#include "ModelShader.h"

class ToonShader : public ModelShader
{
private:
	const char* VertexShaderName = "Data/Shader/ToonVS.cso";
	const char* PixelShaderName = "Data/Shader/ToonPS.cso";
public:
	ToonShader(ID3D11Device* device);
	~ToonShader() override = default;
private:
	// �����_�[�X�e�[�g�ݒ�
	void SetRenderState(const RenderContext& rc) override;
	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	void SetShaderResourceView(const ModelResource::Mesh& mesh, ID3D11DeviceContext*& dc) override;
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> toontexture;
};