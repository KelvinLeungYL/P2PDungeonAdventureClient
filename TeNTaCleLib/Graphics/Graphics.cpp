#include "Misc.h"
#include "Graphics.h"
#include "PhongShader.h"
#include "ToonShader.h"
#include "SkydomeShader.h"

#include "UVScrollShader.h"
#include "MaskShader.h"
#include "ColorGradingShader.h"
#include "GaussianBlurShader.h"
#include "LuminanceExtractionShader.h"
#include "FinalpassShader.h"

// 初期化
void Graphics::Initalize(HWND hWnd)
{
	// ADAPTER
	IDXGIFactory* factory;
	CreateDXGIFactory(IID_PPV_ARGS(&factory));
	IDXGIAdapter* adapter;
	for (UINT adapter_index = 0; S_OK == factory->EnumAdapters(adapter_index, &adapter); ++adapter_index) {
		DXGI_ADAPTER_DESC adapter_desc;
		adapter->GetDesc(&adapter_desc);
		if (adapter_desc.VendorId == 0x1002/*AMD*/ || adapter_desc.VendorId == 0x10DE/*NVIDIA*/)
		{
			break;
		}
		adapter->Release();
	}
	if (adapter == nullptr)
	{
		factory->EnumAdapters(0, &adapter);
		DXGI_ADAPTER_DESC adapter_desc;
		adapter->GetDesc(&adapter_desc);
	}
	factory->Release();

	// 画面サイズ
	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT screenWidth = rc.right - rc.left;
	UINT screenHeight = rc.bottom - rc.top;

	this->screenWidth = static_cast<float>(screenWidth);
	this->screenHeight = static_cast<float>(screenHeight);

	HRESULT hr = S_OK;

	// デバイス&スワップチェーン生成
	{
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1,
		};

		// スワップチェーン設定
		DXGI_SWAP_CHAIN_DESC swapchainDesc;
		{
			swapchainDesc.BufferDesc.Width = screenWidth;
			swapchainDesc.BufferDesc.Height = screenHeight;
			swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;

			swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			swapchainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapchainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapchainDesc.SampleDesc.Count = 1;
			swapchainDesc.SampleDesc.Quality = 0;
			swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapchainDesc.BufferCount = 1;
			swapchainDesc.OutputWindow = hWnd;
			swapchainDesc.Windowed = TRUE;
			swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			swapchainDesc.Flags = 0;
		}

		D3D_FEATURE_LEVEL featureLevel;

		// デバイス&スワップチェーン生成
		hr = D3D11CreateDeviceAndSwapChain(
			// nullptr,l/
			adapter,
			//D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_UNKNOWN,
			nullptr,
			createDeviceFlags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&swapchainDesc,
			swapchain.GetAddressOf(),
			device.GetAddressOf(),
			&featureLevel,
			immediateContext.GetAddressOf()
		);

		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	adapter->Release();

	// フレームバッファ作成
	frameBuffers[static_cast<int>(FrameBufferId::Display)] = std::make_unique<FrameBuffer>(device.Get(), swapchain.Get());
	frameBuffers[static_cast<int>(FrameBufferId::Scene)] = std::make_unique<FrameBuffer>(device.Get(), screenWidth, screenHeight);
	frameBuffers[static_cast<int>(FrameBufferId::Luminance)] = std::make_unique<FrameBuffer>(device.Get(), screenWidth / 2, screenHeight / 2);
	frameBuffers[static_cast<int>(FrameBufferId::GaussianBlur)] = std::make_unique<FrameBuffer>(device.Get(), screenWidth / 2, screenHeight / 2);

	// レンダーステート作成
	renderState = std::make_unique<RenderState>(device.Get());

	// ギズモ生成
	gizmos = std::make_unique<Gizmos>(device.Get());

	// モデルシェーダー生成
	modelShaders[static_cast<int>(ModelShaderId::Phong)] = std::make_unique<PhongShader>(device.Get());
	modelShaders[static_cast<int>(ModelShaderId::Toon)] = std::make_unique<ToonShader>(device.Get());
	modelShaders[static_cast<int>(ModelShaderId::Skydome)] = std::make_unique<SkydomeShader>(device.Get());

	// スプライトシェーダー生成
	spriteShaders[static_cast<int>(SpriteShaderId::Default)] = std::make_unique<DefaultSpriteShader>(device.Get());
	spriteShaders[static_cast<int>(SpriteShaderId::UVScroll)] = std::make_unique<UVScrollShader>(device.Get());
	spriteShaders[static_cast<int>(SpriteShaderId::Mask)] = std::make_unique<MaskShader>(device.Get());
	spriteShaders[static_cast<int>(SpriteShaderId::ColorGrading)] = std::make_unique<ColorGradingShader>(device.Get());
	spriteShaders[static_cast<int>(SpriteShaderId::GaussianBlur)] = std::make_unique<GaussianBlurShader>(device.Get());
	spriteShaders[static_cast<int>(SpriteShaderId::LuminanceExtraction)] = std::make_unique<LuminanceExtractionShader>(device.Get());
	spriteShaders[static_cast<int>(SpriteShaderId::Finalpass)] = std::make_unique<FinalpassShader>(device.Get());

	// レンダラ
	debugRenderer = std::make_unique<DebugRenderer>(device.Get());
	lineRenderer = std::make_unique<LineRenderer>(device.Get(), 1024);
}

void Graphics::Present(UINT syncInterval)
{
	swapchain->Present(syncInterval, 0);
}

DirectX::XMFLOAT3 Graphics::GetScreenPosition(
	const DirectX::XMFLOAT3 worldPosition,
	const D3D11_VIEWPORT& viewport,
	const  DirectX::XMMATRIX& View,
	const  DirectX::XMMATRIX& Projection,
	const DirectX::XMMATRIX World
)
{
	DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
		DirectX::XMVectorSet(worldPosition.x, worldPosition.y, worldPosition.z, 0),
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		viewport.MinDepth,
		viewport.MaxDepth,
		Projection,
		View,
		World
	);
	DirectX::XMFLOAT3 screenPosition;
	DirectX::XMStoreFloat3(&screenPosition, ScreenPosition);
	return screenPosition;
}
DirectX::XMFLOAT3 Graphics::GetScreenPosition(const DirectX::XMFLOAT3 worldPosition)
{
	// ビューボード
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	immediateContext.Get()->RSGetViewports(&numViewports, &viewport);

	// 変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&Camera::Instance().GetView());
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	return GetScreenPosition(worldPosition, viewport, View, Projection, World);
}