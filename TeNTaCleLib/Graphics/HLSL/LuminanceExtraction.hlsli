#include "FilterFunctions.hlsli"
#include "Sprite.hlsli"

cbuffer CBLuminanceExtraction : register(b0)
{
    float threshold; // ���P�x���o�̂��߂�臒l
    float intensity; // �u���[���̋��x
    float2 dummy;
};
