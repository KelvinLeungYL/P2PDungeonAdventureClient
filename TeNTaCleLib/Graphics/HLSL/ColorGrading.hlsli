#include "FilterFunctions.hlsli"
#include "Sprite.hlsli"

//	�X�v���C�g�`��P�ʂŎw�肷��p�����[�^�[
cbuffer CBColorGrading : register(b0)
{
    float hueShift; // �F������
    float saturation; // �ʓx����
    float brightness; // ���x����
    float dummy;
};
