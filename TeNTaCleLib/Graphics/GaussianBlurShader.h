#pragma once
#include "SpriteShader.h"
class GaussianBlurShader : public SpriteShader
{
public:
	GaussianBlurShader(ID3D11Device* device);

private:
	struct CBFilter
	{
		DirectX::XMFLOAT4 weights[MaxKernelSize * MaxKernelSize];
		float kernelSize;
		DirectX::XMFLOAT2 texcel;
		float dummy;
	};
private:
	void UpdateConstantBuffer(const RenderContext& rc) override;
	// �t�B���^�[�l�v�Z
	void CalcGaussianFilter(CBFilter& cbFilter, const GaussianFilterData& gaussianFilterData);
};
