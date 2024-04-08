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

// グラフィックス
class Graphics
{
private:
	Graphics() = default;
	~Graphics() = default;
public:
	// インスタンス
	static Graphics& Instance()
	{
		static Graphics instance;
		return instance;
	}

	// 初期化
	void Initalize(HWND hWnd);
	// 画面表示
	void Present(UINT syncInterval);
	// デバイス取得
	ID3D11Device* GetDevice() { return device.Get(); };
	// デバイスコンテキスト取得
	ID3D11DeviceContext* GetDeviceContext() { return immediateContext.Get(); }
	// スクリーン幅取得
	float GetScreenWidth() const { return screenWidth; }
	// スクリーン高さ取得
	float GetScreenHeight() const { return screenHeight; }
	// フレームバッファ取得
	FrameBuffer* GetFrameBuffer(FrameBufferId frameBufferId) { return frameBuffers[static_cast<int>(frameBufferId)].get(); }
	// レンダーステート取得
	RenderState* GetRenderState() { return renderState.get(); }
	// ギズモ取得
	Gizmos* GetGizmos() { return gizmos.get(); }
	// ミューテックス取得
	std::mutex& GetMutex() { return mutex; }

	// モデルシェーダー取得
	ModelShader* GetModelShader(ModelShaderId shaderId) { return modelShaders[static_cast<int>(shaderId)].get(); }
	// スプライトシェーダー取得
	SpriteShader* GetSpriteShader(SpriteShaderId shaderId) { return spriteShaders[static_cast<int>(shaderId)].get(); }
	// デバッグレンダラ取得
	DebugRenderer* GetDebugRenderer() const { return debugRenderer.get(); }
	// ラインレンダラ取得
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

	std::mutex mutex;	// ミューテックス
};
