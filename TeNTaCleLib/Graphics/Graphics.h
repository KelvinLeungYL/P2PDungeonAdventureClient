#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <memory>
#include <mutex>
#include "FrameBuffer.h"
#include "RenderState.h"
#include "Gizmos.h"
#include "ModelShader.h"
#include "SpriteShader.h"
#include "Graphics/DebugRenderer.h"
#include "Graphics/LineRenderer.h"

enum class ModelShaderId
{
	Phong,
	Toon,
	Skydome,

	EnumCount
};

enum class SpriteShaderId
{
	Default,
	UVScroll,
	Mask,
	ColorGrading,
	GaussianBlur,
	LuminanceExtraction,
	Finalpass,

	EnumCount
};

enum class FrameBufferId
{
	Display,
	Scene,
	Luminance,
	GaussianBlur,

	EnumCount
};

// �O���t�B�b�N�X
class Graphics
{
private:
	Graphics() = default;
	~Graphics() = default;
public:
	// �C���X�^���X
	static Graphics& Instance()
	{
		static Graphics instance;
		return instance;
	}

	// ������
	void Initalize(HWND hWnd);
	// ��ʕ\��
	void Present(UINT syncInterval);
	// �f�o�C�X�擾
	ID3D11Device* GetDevice() { return device.Get(); };
	// �f�o�C�X�R���e�L�X�g�擾
	ID3D11DeviceContext* GetDeviceContext() { return immediateContext.Get(); }
	// �X�N���[�����擾
	float GetScreenWidth() const { return screenWidth; }
	// �X�N���[�������擾
	float GetScreenHeight() const { return screenHeight; }
	// �t���[���o�b�t�@�擾
	FrameBuffer* GetFrameBuffer(FrameBufferId frameBufferId) { return frameBuffers[static_cast<int>(frameBufferId)].get(); }
	// �����_�[�X�e�[�g�擾
	RenderState* GetRenderState() { return renderState.get(); }
	// �M�Y���擾
	Gizmos* GetGizmos() { return gizmos.get(); }
	// �~���[�e�b�N�X�擾
	std::mutex& GetMutex() { return mutex; }

	// ���f���V�F�[�_�[�擾
	ModelShader* GetModelShader(ModelShaderId shaderId) { return modelShaders[static_cast<int>(shaderId)].get(); }
	// �X�v���C�g�V�F�[�_�[�擾
	SpriteShader* GetSpriteShader(SpriteShaderId shaderId) { return spriteShaders[static_cast<int>(shaderId)].get(); }
	// �f�o�b�O�����_���擾
	DebugRenderer* GetDebugRenderer() const { return debugRenderer.get(); }
	// ���C�������_���擾
	LineRenderer* GetLineRenderer() const { return lineRenderer.get(); }

	DirectX::XMFLOAT3 GetScreenPosition(
		const DirectX::XMFLOAT3 worldPosition,
		const D3D11_VIEWPORT& viewport,
		const  DirectX::XMMATRIX& View,
		const  DirectX::XMMATRIX& Projection,
		const DirectX::XMMATRIX World
	);
	DirectX::XMFLOAT3 GetScreenPosition(const DirectX::XMFLOAT3 worldPosition);
private:
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediateContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;

	float screenWidth;
	float screenHeight;
	std::unique_ptr<FrameBuffer> frameBuffers[static_cast<int>(FrameBufferId::EnumCount)];
	std::unique_ptr<RenderState> renderState;
	std::unique_ptr<Gizmos> gizmos;

	std::unique_ptr<ModelShader> modelShaders[static_cast<int>(ModelShaderId::EnumCount)];
	std::unique_ptr<SpriteShader> spriteShaders[static_cast<int>(SpriteShaderId::EnumCount)];

	std::unique_ptr<DebugRenderer>					debugRenderer;
	std::unique_ptr<LineRenderer>					lineRenderer;

	std::mutex mutex;	// �~���[�e�b�N�X
};
