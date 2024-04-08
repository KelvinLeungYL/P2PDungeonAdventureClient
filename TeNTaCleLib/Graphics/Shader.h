#pragma once

#include "RenderContext.h"
#include "Model.h"
#include "Sprite.h"

class Shader
{
public:
	Shader() {}
	virtual ~Shader() {}

	// •`‰æŠJŽn
	virtual void Begin(const RenderContext& rc) = 0;

	// •`‰æI—¹
	virtual void End(const RenderContext& rc)
	{
		ID3D11DeviceContext* dc = rc.deviceContext;
		dc->VSSetShader(nullptr, nullptr, 0);
		dc->PSSetShader(nullptr, nullptr, 0);
		dc->IASetInputLayout(nullptr);
		ID3D11ShaderResourceView* srvs[] = { nullptr, nullptr };
		dc->PSSetShaderResources(0, ARRAYSIZE(srvs), srvs);
	}
};
