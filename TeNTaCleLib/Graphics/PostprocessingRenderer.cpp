#include <imgui.h>
#include "PostprocessingRenderer.h"
#include "tentacle_lib.h"

PostprocessingRenderer::PostprocessingRenderer()
{
	Graphics& graphics = Graphics::Instance();

	renderSprite = std::make_unique<Sprite>(graphics.GetDevice());

	bloomData.gaussianFilterData.textureSize.x = graphics.GetScreenWidth() / 2.0f;
	bloomData.gaussianFilterData.textureSize.y = graphics.GetScreenHeight() / 2.0f;
}

PostprocessingRenderer::~PostprocessingRenderer()
{
}

void PostprocessingRenderer::Render(ID3D11DeviceContext* deviceContext)
{
	// ���ݐݒ肳��Ă���o�b�t�@��ޔ����ď���������
	UINT			cachedViewportCount{ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE };
	D3D11_VIEWPORT	cachedViewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	cachedRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	cachedDepthStencilView;
	{
		deviceContext->RSGetViewports(&cachedViewportCount, cachedViewports);
		deviceContext->OMGetRenderTargets(
			1,
			cachedRenderTargetView.ReleaseAndGetAddressOf(),
			cachedDepthStencilView.ReleaseAndGetAddressOf()
		);
	}

	RenderContext	rc;
	rc.deviceContext = deviceContext;
	rc.renderState = T_GRAPHICS.GetRenderState();
	rc.luminanceExtractionData = bloomData.luminanceExtractionData;
	rc.gaussianFilterData = bloomData.gaussianFilterData;

	//	���P�x���o�p�o�b�t�@�ɕ`����ύX���č��P�x���o
	{
		//	�`����ύX
		T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->Clear(T_GRAPHICS.GetDeviceContext(), 0.0f, 0.0f, 0.0f, 0.0f);
		T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->SetRenderTargets(T_GRAPHICS.GetDeviceContext());

		rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);

		//	���P�x���o����
		SpriteShader* shader = T_GRAPHICS.GetSpriteShader(SpriteShaderId::LuminanceExtraction);
		shader->Begin(rc);
		//	�`��Ώۂ�ύX
		renderSprite->SetShaderResourceView(
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::Scene)->GetShaderResourceView(),
			static_cast<UINT>(T_GRAPHICS.GetScreenWidth()),
			static_cast<UINT>(T_GRAPHICS.GetScreenHeight())
		);
		renderSprite->Update(
			0, 0,
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->GetViewport().Width,
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->GetViewport().Height,
			0, 0,
			T_GRAPHICS.GetScreenWidth(), T_GRAPHICS.GetScreenHeight(),
			0,
			1, 1, 1, 1
		);
		shader->Draw(rc, renderSprite.get());
		shader->End(rc);
	}

	// ���o�������P�x�`��Ώۂ�򂵂ď�������
	{
		//	�`����ύX
		T_GRAPHICS.GetFrameBuffer(FrameBufferId::GaussianBlur)->Clear(T_GRAPHICS.GetDeviceContext(), 0.0f, 0.0f, 0.0f, 0.0f);
		T_GRAPHICS.GetFrameBuffer(FrameBufferId::GaussianBlur)->SetRenderTargets(T_GRAPHICS.GetDeviceContext());

		rc.deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);

		//	���P�x���o����
		SpriteShader* shader = T_GRAPHICS.GetSpriteShader(SpriteShaderId::GaussianBlur);
		shader->Begin(rc);

		//	�`��Ώۂ�ύX
		renderSprite->SetShaderResourceView(
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->GetShaderResourceView(),
			static_cast<UINT>(T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->GetViewport().Width),
			static_cast<UINT>(T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->GetViewport().Height)
		);
		renderSprite->Update(
			0, 0,
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::GaussianBlur)->GetViewport().Width,
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::GaussianBlur)->GetViewport().Height,
			0, 0,
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->GetViewport().Width,
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::Luminance)->GetViewport().Height,
			0,
			1, 1, 1, 1
		);
		shader->Draw(rc, renderSprite.get());
		shader->End(rc);
	}

	//	���̃o�b�t�@�ɖ߂�
	{
		deviceContext->RSSetViewports(cachedViewportCount, cachedViewports);
		deviceContext->OMSetRenderTargets(1, cachedRenderTargetView.GetAddressOf(), cachedDepthStencilView.Get());
	}

	// ���o�������P�x�`��Ώۂ���������
	{
		//	���P�x���o����
		SpriteShader* shader = T_GRAPHICS.GetSpriteShader(SpriteShaderId::Finalpass);
		shader->Begin(rc);

		renderSprite->SetShaderResourceView(
			T_GRAPHICS.GetFrameBuffer(FrameBufferId::Scene)->GetShaderResourceView(),
			static_cast<UINT>(T_GRAPHICS.GetScreenWidth()),
			static_cast<UINT>(T_GRAPHICS.GetScreenHeight())
		);
		renderSprite->Update(
			-1, -1,
			T_GRAPHICS.GetScreenWidth() + 2, T_GRAPHICS.GetScreenHeight() + 2,
			0, 0,
			T_GRAPHICS.GetScreenWidth(), T_GRAPHICS.GetScreenHeight(),
			0,
			1, 1, 1, 1
		);

		//	�V�F�[�_�[�ɓn���e�N�X�`����ݒ�
		rc.finalpassnData.bloomTexture = T_GRAPHICS.GetFrameBuffer(FrameBufferId::GaussianBlur)->GetShaderResourceView().Get();
		rc.colorGradingData = colorGradingData;

		shader->Draw(rc, renderSprite.get());

		shader->End(rc);
	}
}

//	�f�o�b�O���̕\��
void PostprocessingRenderer::DrawDebugGUI()
{
	if (ImGui::TreeNode("PostProcess"))
	{
		if (ImGui::TreeNode("Bloom"))
		{
			ImGui::SliderFloat("threshold", &bloomData.luminanceExtractionData.threshold, 0.0f, 1.0f);
			ImGui::SliderFloat("intensity", &bloomData.luminanceExtractionData.intensity, 0.0f, 10.0f);
			ImGui::SliderInt("kernelSize", &bloomData.gaussianFilterData.kernelSize, 1, MaxKernelSize - 1);
			ImGui::SliderFloat("deviation", &bloomData.gaussianFilterData.deviation, 1.0f, 10.0f);
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("ColorGrading"))
		{
			ImGui::SliderFloat("hueShift", &colorGradingData.hueShift, 0.0f, 360.0f);
			ImGui::SliderFloat("saturation", &colorGradingData.saturation, 0.0f, 2.0f);
			ImGui::SliderFloat("brightness", &colorGradingData.brightness, 0.0f, 2.0f);
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}