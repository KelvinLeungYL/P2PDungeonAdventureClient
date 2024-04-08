#pragma once
#include "SpriteShader.h"

class FinalpassShader : public SpriteShader
{
public:
	FinalpassShader(ID3D11Device* device);

	void Draw(const RenderContext& rc, const Sprite* sprite) override;
private:
	void UpdateConstantBuffer(const RenderContext& rc) override;
private:
	struct CBFinalpass
	{
		//	�F���␳
		float	hueShift;	// �F������
		float	saturation;	// �ʓx����
		float	brightness;	// ���x����
		float	dummy;
	};
};
