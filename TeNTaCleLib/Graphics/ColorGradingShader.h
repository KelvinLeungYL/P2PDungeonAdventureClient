#pragma once
#include "SpriteShader.h"
class ColorGradingShader : public SpriteShader
{
public:
	ColorGradingShader(ID3D11Device* device);
private:
	void UpdateConstantBuffer(const RenderContext& rc) override;
private:
	struct CBColorGrading
	{
		float	hueShift;	// �F������
		float	saturation;	// �ʓx����
		float	brightness;	// ���x����
		float	dummy;
	};
};
