#pragma once

#include <DirectXMath.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <unordered_map>
#include "Singleton.h"
#include "Effect.h"

class EffectManager : public Singleton<EffectManager>
{
	friend class Singleton<EffectManager>;
private:
	EffectManager() {};
	~EffectManager() {};

public:
	Effect* GetEffect(int index);

	void Initialize();				// ������
	void Finalize();				// �I��
	void Update(float elapsedTime);	//�X�V
	void Render(const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection); // �`��

	// Effeckseer�}�l�[�W���[�̎擾
	Effekseer::ManagerRef GetEffekseerManager() { return effekseerManager; }
public:
	enum EFFECT_IDX {
		HIT_SLASH_EFFECT,
		HIT_SHOT_EFFECT,
		BOMB_EFFECT,
		FORCE_EFFECT,
	};
private:
	Effekseer::ManagerRef effekseerManager;
	EffekseerRenderer::RendererRef effekseerRenderer;

	std::unordered_map<int, Effect*> effects;
};

#define EFFECTS EffectManager::Instance()