#pragma once
#include <DirectXMath.h>

class Mathf
{
public:
	// ���`�⊮
	static float Lerp(float a, float b, float t);
	// �w��͈͂̃����_���l���v�Z����
	static float RandomRange(float min, float max);
	// �덷��r
	static bool cmpf(float a, float b, float epsilon = 0.005f);
	static bool cmpfloat3(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b, float epsilon = 0.005f);
};