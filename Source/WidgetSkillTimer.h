#pragma once

#include <memory>

#include "Widget.h"
#include "Player.h"

class WidgetSkillTimer : public Widget
{
public:
	WidgetSkillTimer();

	void Render(const RenderContext& rc);
private:
	enum ICON {
		ICON_ROCK,			// �X�L���A�C�R��_��.png
		ICON_FLAG,			// �X�L���A�C�R��_��.png
		ICON_HAND,			// �X�L���A�C�R��_��.png
		ICON_BOOK,			// �X�L���A�C�R��_�{.png
		ICON_ARROW,			// �X�L���A�C�R��_��.png
		ICON_WING,			// �X�L���A�C�R��_��.png
		ICON_BUG,			// �X�L���A�C�R��_�o�O.png
		ICON_BUFF,			// �X�L���A�C�R��_�o�t.png
		ICON_TALK,			// �X�L���A�C�R��_�t�L�_�V.png

		ICON_FEATHER,		// �X�L���A�C�R��_�H��.png
		ICON_GEAR,			// �X�L���A�C�R��_����.png
		ICON_HEAL,			// �X�L���A�C�R��_����.png
		ICON_EXPERIMENT,	// �X�L���A�C�R��_����.png
		ICON_IMPACT,		// �X�L���A�C�R��_�Ռ�.png
		ICON_SNIPER,		// �X�L���A�C�R��_�_��.png
		ICON_NINJA,			// �X�L���A�C�R��_�E�p.png
		ICON_BOMB,			// �X�L���A�C�R��_���e.png
		ICON_RELOAD,		// �X�L���A�C�R��_�����[�h.png

		ICON_BLOCK,			// �X�L���A�C�R��_�h��.png
		ICON_NONE,			// �X�L���A�C�R��_���n.png
		ICON_SLASH_1,		// �X�L���A�C�R��_�a��1.png
		ICON_MAGIC_1,		// �X�L���A�C�R��_���@1.png
		ICON_MAGIC_10,		// �X�L���A�C�R��_���@10.png
		ICON_MAGIC_11,		// �X�L���A�C�R��_���@11.png
		ICON_MAGIC_12,		// �X�L���A�C�R��_���@12.png
		ICON_MAGIC_13,		// �X�L���A�C�R��_���@13.png
		ICON_CC,			// �X�L���A�C�R��_��Ԉُ�.png

		ICON_MAGIC_14,		// �X�L���A�C�R��_���@14.png
		ICON_SLASH_2,		// �X�L���A�C�R��_�a��2.png
		ICON_MAGIC_2,		// �X�L���A�C�R��_���@2.png
		ICON_SLASH_3,		// �X�L���A�C�R��_�a��3.png
		ICON_MAGIC_3,		// �X�L���A�C�R��_���@3.png
		ICON_SLASH_4,		// �X�L���A�C�R��_�a��4.png
		ICON_MAGIC_4,		// �X�L���A�C�R��_���@4.png
		ICON_SLASH_5,		// �X�L���A�C�R��_�a��5.png
		ICON_AIR_ATTACK,	// �X�L���A�C�R��_�΋�U��.png

		ICON_MAGIC_5,		// �X�L���A�C�R��_���@5.png
		ICON_SLASH_6,		// �X�L���A�C�R��_�a��6.png
		ICON_MAGIC_6,		// �X�L���A�C�R��_���@6.png
		ICON_MAGIC_7,		// �X�L���A�C�R��_���@7.png
		ICON_MAGIC_8,		// �X�L���A�C�R��_���@8.png
		ICON_MAGIC_9,		// �X�L���A�C�R��_���@9.png
		ICON_MIKO,			// �X�L���A�C�R��_���P��.png
		ICON_DEBUFF,		// �X�L���A�C�R��_�f�o�t.png
		ICON_SCYTHE,		// �X�L���A�C�R��_�劙�U��.png

		ICON_SKULL,			// �X�L���A�C�R��_�h�N��.png
		ICON_PUNCH,			// �X�L���A�C�R��_�p���`.png
		ICON_BREATH,		// �X�L���A�C�R��_�u���X.png
		ICON_RING,			// �X�L���A�C�R��_�����O.png
		ICON_HEALER,		// �X�L���A�C�R��_�񕜖�.png
		ICON_BOW,			// �X�L���A�C�R��_�|�U��.png
		ICON_SWORD,			// �X�L���A�C�R��_���U��.png
		ICON_RRIDENT,		// �X�L���A�C�R��_�O����.png
		ICON_CLAYMORE,		// �X�L���A�C�R��_�匕�U��.png

		ICON_GUN,			// �X�L���A�C�R��_�e�U��.png
		ICON_LAMP,			// �X�L���A�C�R��_�Ƃ炷.png
		ICON_HAMMER,		// �X�L���A�C�R��_�ƍU��.png
		ICON_KATANA,		// �X�L���A�C�R��_���U��.png
		ICON_AXE,			// �X�L���A�C�R��_���U��.png
		ICON_ITEM_BAG,		// �X�L���A�C�R��_�����.png
		ICON_SPECIAL_1,		// �X�L���A�C�R��_�K�E�Z1.png
		ICON_SPECIAL_10,	// �X�L���A�C�R��_�K�E�Z10.png
		ICON_SUMMON,		// �X�L���A�C�R��_��������.png

		ICON_SPECIAL_2,		// �X�L���A�C�R��_�K�E�Z2.png
		ICON_SPECIAL_3,		// �X�L���A�C�R��_�K�E�Z3.png
		ICON_SPECIAL_4,		// �X�L���A�C�R��_�K�E�Z4.png
		ICON_SPECIAL_5,		// �X�L���A�C�R��_�K�E�Z5.png
		ICON_SPECIAL_6,		// �X�L���A�C�R��_�K�E�Z6.png
		ICON_SPECIAL_7,		// �X�L���A�C�R��_�K�E�Z7.png
		ICON_SPECIAL_8,		// �X�L���A�C�R��_�K�E�Z8.png
		ICON_SPECIAL_9,		// �X�L���A�C�R��_�K�E�Z9.png
		ICON_MAGIC_EFFECT,	// �X�L���A�C�R��_���@����.png

		ICON_MAGIC_ATTACK,	// �X�L���A�C�R��_���@�U��.png
		ICON_COUNTER,		// �X�L���A�C�R��_�J�E���^�[.png
		ICON_KNIFE,			// �X�L���A�C�R��_�i�C�t�U��.png
		ICON_LOCKON,		// �X�L���A�C�R��_���b�N�I��.png
		ICON_HARISEN,		// �X�L���A�C�R��_�n���Z���U��.png
		ICON_HALBERD,		// �X�L���A�C�R��_�n���o�[�h�U��.png
	};
	const int lineBreak = 9;

private:
	void DrawIcon(
		const RenderContext& rc,
		const int& imageIdx,
		const DirectX::XMFLOAT2& pos,
		int skillTimerIdx,
		const char* key
	);
private:
	Player* player = nullptr;
	std::shared_ptr<Sprite> skillIcons;
	std::shared_ptr<Sprite> mask;
};
