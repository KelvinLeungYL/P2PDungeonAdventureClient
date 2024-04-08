#pragma once

#include "Graphics.h"
#include "Sprite.h"

// �|�X�g�v���Z�X�p�̃����_���[
class PostprocessingRenderer
{
	// �u���[�����
	struct BloomData
	{
		//	���P�x���o�p���
		LuminanceExtractionData	luminanceExtractionData;
		//	�K�E�X�u���[�p���
		GaussianFilterData		gaussianFilterData;
	};

public:
	PostprocessingRenderer();
	~PostprocessingRenderer();

	//	�`��
	void Render(ID3D11DeviceContext* deviceContext);

	//	�f�o�b�O���̕\��
	void DrawDebugGUI();

	void SetBloom(const BloomData& bd) {
		bloomData.gaussianFilterData.deviation = bd.gaussianFilterData.deviation;
		bloomData.gaussianFilterData.kernelSize = bd.gaussianFilterData.kernelSize;
		bloomData.luminanceExtractionData.intensity = bd.luminanceExtractionData.intensity;
		bloomData.luminanceExtractionData.threshold = bd.luminanceExtractionData.threshold;
	}
	void SetColoGrading(const ColorGradingData& cg) { colorGradingData = cg; }
private:
	// �`��p�X�v���C�g
	std::unique_ptr<Sprite>			renderSprite;

	// �u���[���f�[�^
	BloomData						bloomData;

	// �F���␳�f�[�^
	ColorGradingData				colorGradingData;
};
