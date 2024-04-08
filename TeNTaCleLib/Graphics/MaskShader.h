#pragma once
#include "SpriteShader.h"
class MaskShader : public SpriteShader
{
public:
	MaskShader(ID3D11Device* device);
private:
	void UpdateConstantBuffer(const RenderContext& rc) override;
private:
	// 臒l�𑗂邽�߂̒萔�o�b�t�@�p�\����
	struct CbMask
	{
		float			  dissolveThreshold;	// �f�B�]���u��
		float			  edgeThreshold; 		// ����臒l
		DirectX::XMFLOAT2 dummy;
		DirectX::XMFLOAT4 edgeColor;			// ���̐F
	};
};
