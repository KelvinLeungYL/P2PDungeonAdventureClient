#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <DirectXMath.h>

class Sprite
{
public:
	Sprite(ID3D11Device* device);
	Sprite(ID3D11Device* device, const char* filename);

	const DirectX::XMFLOAT2 GetTextureSize() const { return textureSize; }

	// ���_�f�[�^
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
		DirectX::XMFLOAT2 texcoord;
	};

	// �o�b�t�@�X�V
	virtual void Update(
		float dx, float dy,
		float dw, float dh,
		float sx, float sy,
		float sw, float sh,
		float angle,
		float r, float g, float b, float a) const;

	// �`����s
	void Render(
		ID3D11DeviceContext* dc,
		float dx, float dy,					// �ʒu
		float dz,							// ���s
		float dw, float dh,					// �T�C�Y
		float sx, float sy,					// �摜�؂蔲���ʒu
		float sw, float sh,					// �摜�؂蔲���T�C�Y
		float angle,						// �p�x
		float r, float g, float b, float a	// �F
	) const;

	// �`����s (�e�N�X�`���؂蔲���w��Ȃ�)
	void Render(
		ID3D11DeviceContext* dc,
		float dx, float dy,					// �ʒu
		float dz,							// ���s
		float dw, float dh,					// �T�C�Y
		float angle,						// �p�x
		float r, float g, float b, float a	// �F
	) const;
	// �`����s (�e�N�X�`���؂蔲���w��Ȃ��F�p�x�w��Ȃ�)
	void Render(
		ID3D11DeviceContext* dc,
		float dx, float dy,					// �ʒu
		float dz,							// ���s
		float dw, float dh					// �T�C�Y
	) const;

	// �V�F�[�_�[���\�[�X�r���[�̐ݒ�
	void SetShaderResourceView(const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& srv, int texWidth, int texHeight);

	// ���_�o�b�t�@�̎擾
	const Microsoft::WRL::ComPtr<ID3D11Buffer>& GetVertexBuffer() const { return vertexBuffer; }

	// �V�F�[�_�[���\�[�X�r���[�̎擾
	const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const { return shaderResourceView; }
protected:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;

	DirectX::XMFLOAT2 textureSize = {};
};
