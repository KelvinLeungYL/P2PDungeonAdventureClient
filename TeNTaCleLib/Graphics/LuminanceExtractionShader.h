#pragma once
#include "SpriteShader.h"
class LuminanceExtractionShader : public SpriteShader
{
public:
	LuminanceExtractionShader(ID3D11Device* device);
private:
	void UpdateConstantBuffer(const RenderContext& rc) override;
private:
	struct CBLuminanceExtraction
	{
		float				threshold;	// 臒l
		float				intensity;	// �u���[���̋��x
		DirectX::XMFLOAT2	dummy2;
	};
};
