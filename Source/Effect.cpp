#include "tentacle_lib.h"
#include "Effect.h"
#include "EffectManager.h"

// �R���X�g���N�^
Effect::Effect(const char* filename)
{
	std::lock_guard<std::mutex> lock(T_GRAPHICS.GetMutex());
	// Effekseer�̃��\�[�X��ǂݍ���
	// Effekseer��UTF-16�̃t�@�C���p�X�ȊO�͑Ή����Ă��Ȃ����ߕ����R�[�h�ϊ����K�v
	char16_t utf16Filename[256];
	Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);
	// Effekseer�G�t�F�N�g��ǂݍ���
	effekseerEffect = Effekseer::Effect::Create(Manager(), (EFK_CHAR*)utf16Filename);
}
// �Đ�
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
	Effekseer::Handle handle = Manager()->Play(effekseerEffect, position.x, position.y, position.z);
	Manager()->SetScale(handle, scale, scale, scale);
	return handle;
}
// ��~
void Effect::Stop(Effekseer::Handle handle)
{
	Manager()->StopEffect(handle);
}
// ���W�ݒ�
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	Manager()->SetLocation(handle, position.x, position.y, position.z);
	Manager()->SetTargetLocation(handle, position.x, position.y, position.z);
}
// �X�P�[���ݒ�
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	Manager()->SetScale(handle, scale.x, scale.y, scale.z);
}

void Effect::SetRotation(Effekseer::Handle handle, const DirectX::XMFLOAT3& rotation)
{
	Manager()->SetRotation(handle, rotation.x, rotation.y, rotation.z);
}

Effekseer::ManagerRef Effect::Manager() {
	return EFFECTS.GetEffekseerManager();
}