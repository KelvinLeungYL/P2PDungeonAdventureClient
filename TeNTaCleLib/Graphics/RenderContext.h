#pragma once
#include "Camera.h"
#include "RenderState.h"

//	UV�X�N���[�����
struct UVScrollData
{
	DirectX::XMFLOAT2	uvScrollValue;	// UV�X�N���[���l
};

// �}�X�N�f�[�^
struct MaskData
{
	ID3D11ShaderResourceView* maskTexture;
	float dissolveThreshold;
	float edgeThreshold; 			// ����臒l
	DirectX::XMFLOAT4 edgeColor;	// ���̐F
};

struct DirectionalLightData
{
	DirectX::XMFLOAT4 direction;
	DirectX::XMFLOAT4 color;
};

// �_�������
struct PointLightData
{
	DirectX::XMFLOAT4 position;		// ���W
	DirectX::XMFLOAT4 color;		// �F
	float range;					// �͈�
	DirectX::XMFLOAT3 dummy;
};

// �_�����̍ő吔
static constexpr int PointLightMax = 8;
// �X�|�b�g���C�g���
struct SpotLightData
{
	DirectX::XMFLOAT4	position;	// ���W
	DirectX::XMFLOAT4	direction;	// ����
	DirectX::XMFLOAT4	color;		// �F
	float				range;		// �͈�
	float				innerCorn; 	// �C���i�[�p�x�͈�
	float				outerCorn; 	// �A�E�^�[�p�x�͈�
	float				dummy;
};

// �X�|�b�g���C�g�̍ő吔
static	constexpr	int	SpotLightMax = 8;

// �F���␳���
struct ColorGradingData
{
	float	hueShift = 0;	// �F������
	float	saturation = 1;	// �ʓx����
	float	brightness = 1;	// ���x����
	float	dummy;
};

// �K�E�X�t�B���^�[�v�Z���
struct GaussianFilterData
{
	int					kernelSize = 8;		// �J�[�l���T�C�Y
	float				deviation = 10.0f;	// �W���΍�
	DirectX::XMFLOAT2	textureSize;			// �򂷃e�N�X�`���̃T�C�Y
};

// �K�E�X�t�B���^�[�̍ő�J�[�l���T�C�Y
static const int MaxKernelSize = 16;

// ���P�x���o�p���
struct LuminanceExtractionData
{
	float				threshold = 0.5f;	// 臒l
	float				intensity = 1.0f;	// �u���[���̋��x
	DirectX::XMFLOAT2	dummy2;
};

// �|�X�g�G�t�F�N�g�̍ŏI�p�X�p���
struct FinalpassnData
{
	//	�u���[���e�N�X�`��
	ID3D11ShaderResourceView* bloomTexture;
};

struct RenderContext
{
	ID3D11DeviceContext* deviceContext;
	const RenderState* renderState;
	const Camera* camera;

	//	�X�N���[���f�[�^
	UVScrollData			uvScrollData;

	// �}�X�N�f�[�^
	MaskData				maskData;

	//	���C�g���
	DirectX::XMFLOAT4		ambientLightColor;
	DirectionalLightData	directionalLightData;
	PointLightData			pointLightData[PointLightMax];	// �_�������
	SpotLightData			spotLightData[SpotLightMax];	// �X�|�b�g���C�g���
	int						pointLightCount = 0;			// �_������
	int						spotLightCount = 0;				// �X�|�b�g���C�g��

	ColorGradingData		colorGradingData;				//	�F���␳���
	GaussianFilterData		gaussianFilterData;				//	�K�E�X�t�B���^�[���
	LuminanceExtractionData	luminanceExtractionData;		//	���P�x���o�p���
	FinalpassnData			finalpassnData;					//	�ŏI�p�X���
};