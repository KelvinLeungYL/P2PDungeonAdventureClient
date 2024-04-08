#include "WidgetSkillTimer.h"

#include "GameData.h"
#include "PlayerManager.h"
#include "Graphics/ResourceManager.h"
#include "XMFLOAT.h"

WidgetSkillTimer::WidgetSkillTimer()
{
	player = PLAYERS.GetPlayerById(GAME_DATA.GetClientId());
	skillIcons = RESOURCE.LoadSpriteResource("Data/Sprites/skill_icon.png");
	mask = RESOURCE.LoadSpriteResource("");

	size = { 64.0f, 64.0f };
	position = {
		SCREEN_W * 0.5f,
		SCREEN_H - 100.0f
	};
}

void WidgetSkillTimer::Render(const RenderContext& rc)
{
	// �f�t�H���g�F���n
	int skill_1_icon = ICON::ICON_NONE;
	int skill_2_icon = ICON::ICON_NONE;
	int skill_3_icon = ICON::ICON_NONE;
	int skill_4_icon = ICON::ICON_NONE;
	int normal_attack_icon = ICON::ICON_NONE;
	int special_attack_icon = ICON::ICON_NONE;

	switch (player->GetClassType())
	{
	case PLAYER_CLASS::BARBARIAN:
		normal_attack_icon = ICON::ICON_AXE;
		special_attack_icon = ICON::ICON_HAMMER;
		skill_1_icon = ICON::ICON_MAGIC_11;
		break;
	case PLAYER_CLASS::KNIGHT:
		normal_attack_icon = ICON::ICON_SWORD;
		special_attack_icon = ICON::ICON_BLOCK;
		skill_1_icon = ICON::ICON_IMPACT;
		break;
	case PLAYER_CLASS::ROGUE:
		normal_attack_icon = ICON::ICON_ARROW;
		special_attack_icon = ICON::ICON_BOW;
		skill_1_icon = ICON::ICON_BOMB;
		break;
	}

	DrawIcon(rc, skill_1_icon, position + DirectX::XMFLOAT2{ -size.x * 4.0f, 0.0f }, static_cast<int>(Player::State::Skill_1), "[1]");
	DrawIcon(rc, skill_2_icon, position + DirectX::XMFLOAT2{ -size.x * 3.0f, 0.0f }, static_cast<int>(Player::State::Skill_2), "[2]");
	DrawIcon(rc, skill_3_icon, position + DirectX::XMFLOAT2{ -size.x * 2.0f, 0.0f }, static_cast<int>(Player::State::Skill_3), "[3]");
	DrawIcon(rc, skill_4_icon, position + DirectX::XMFLOAT2{ -size.x * 1.0f, 0.0f }, static_cast<int>(Player::State::Skill_4), "[4]");

	DrawIcon(rc, normal_attack_icon, position + DirectX::XMFLOAT2{ 0.0f + 64.0f / 2.0f, 0.0f }, static_cast<int>(Player::State::AttackNormal), "[LMB]");
	DrawIcon(rc, special_attack_icon, position + DirectX::XMFLOAT2{ size.x + 64.0f / 2.0f, 0.0f }, static_cast<int>(Player::State::AttackSpecial), "[RMB]");
}

void WidgetSkillTimer::DrawIcon(
	const RenderContext& rc,
	const int& imageIdx,
	const DirectX::XMFLOAT2& pos,
	int skillTimerIdx,
	const char* key
)
{
	// �w�i
	mask->Render(
		rc.deviceContext,
		pos.x, pos.y, 0.0f,
		size.x, size.y + 20.0f,
		0,
		0.0f, 0.0f, 0.0f, 0.6f
	);
	// �A�C�R��
	float coolTime = player->GetSkillTimerTime(skillTimerIdx);
	skillIcons->Render(
		rc.deviceContext,
		pos.x, pos.y, 0.0f,
		size.x, size.y,
		static_cast<float>(imageIdx % lineBreak) * 512.0f, static_cast<float>(imageIdx / lineBreak) * 512.0f,
		512.0f, 512.0f,
		0,
		1.0f,
		(coolTime > 0.0f) ? 0.4f : 1.0f,
		(coolTime > 0.0f) ? 0.4f : 1.0f,
		1.0f
	);

	// �����E�^�C�}�[
	if (coolTime > 0.0f) // �b���\��
	{
		T_TEXT.Render(
			FONT_ID::MsGothic,
			std::to_string(static_cast<int>(std::ceilf(coolTime))).c_str(),
			pos.x + (size.x * 0.46f), pos.y + size.y + 9.0f,
			1.0f, 0.5f, 0.5f, 1.0f,
			0.0f,
			FONT_ALIGN::CENTER,
			0.4f,
			1,
			{ 1.0f, 0.0f, 0.0f, 1.0f }
		);
	}
	else
	{
		T_TEXT.Render(
			FONT_ID::MsGothic,
			key,
			pos.x + (size.x * 0.46f), pos.y + size.y + 9.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			0.0f,
			FONT_ALIGN::CENTER,
			0.4f,
			1
		);
	}
}