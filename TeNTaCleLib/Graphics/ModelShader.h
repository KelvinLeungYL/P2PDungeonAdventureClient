#pragma once

#include <string>
#include "Shader.h"
#include "ModelResource.h"

class ModelShader : public Shader
{
public:
	ModelShader(ID3D11Device* device, const char* vs, const char* ps);
	virtual ~ModelShader() override = default;

	// �`��J�n
	virtual void Begin(const RenderContext& rc) override;

	// ���f���`��
	virtual void Draw(const RenderContext& rc, const Model* model, DirectX::XMFLOAT4 color = { 1, 1, 1, 1 });

protected:
	// �����_�[�X�e�[�g�ݒ�
	virtual void SetRenderState(const RenderContext& rc) = 0;
	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	virtual void SetShaderResourceView(const ModelResource::Mesh& mesh, ID3D11DeviceContext*& dc) = 0;
protected:
	struct CbScene
	{
		DirectX::XMFLOAT4		cameraPosition;
		DirectX::XMFLOAT4X4		viewProjection;
		DirectX::XMFLOAT4		ambientLightColor;
		DirectionalLightData	directionalLightData;
		PointLightData			pointLightData[PointLightMax];	// �_����
		SpotLightData			spotLightData[SpotLightMax];	// �X�|�b�g���C�g
		int						pointLightCount;				// �_������
		int						spotLightCount;					// �X�|�b�g���C�g��
		DirectX::XMFLOAT2		dummy;
	};

	struct CbMesh
	{
		DirectX::XMFLOAT4 materialColor;
		float linearGamma;								// ���j�A�␳
		DirectX::XMFLOAT3 dummy;
	};

	struct CbSkeleton
	{
		DirectX::XMFLOAT4X4 boneTransforms[256];
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> sceneConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> meshConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11Buffer> skeletonConstantBuffer;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> toontexture;
};
