#pragma once

#include "Shader.h"
#include "Sprite.h"

class SpriteShader : public Shader
{
public:
	SpriteShader(ID3D11Device* device, const char* vs, const char* ps);
	virtual ~SpriteShader() override = default;

	// •`‰æŠJŽn
	virtual void Begin(const RenderContext& rc) override;

	// ƒ‚ƒfƒ‹•`‰æ
	virtual void Draw(const RenderContext& rc, const Sprite* sprite);

protected:
	virtual void UpdateConstantBuffer(const RenderContext& rc) {};
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer>				constantBuffer;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>			vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			inputLayout;
};

class DefaultSpriteShader : public SpriteShader
{
public:
	DefaultSpriteShader(ID3D11Device* device) : SpriteShader(device, "Data/Shader/SpriteVS.cso", "Data/Shader/SpritePS.cso") {}
};