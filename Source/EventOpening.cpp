#include "EventOpening.h"

#include "DialogEvent.h"
#include "CharacterMoveEvent.h"
#include "FlagEvent.h"

EventOpening::EventOpening(PlayableCharacter* mario, PlayableCharacter* luig)
{
	this->mario = mario;
	this->luig = luig;
	actions.emplace_back(new CharacterVisible(mario, false));
	actions.emplace_back(new CharacterPosition(luig, { 500.0f, 31.0f, -72.131f }));
	actions.emplace_back(new CharacterRotate(luig, { 0.0f, 0.0f, 0.0f }));
	actions.emplace_back(new CharacterPosition(mario, { 98.0f, 31.0f, -72.131f }));
	actions.emplace_back(new CharacterRotate(mario, { 0.0f, DirectX::XMConvertToRadians(90), 0.0f }));

	actions.emplace_back(new DialogEvent("", "���̂��\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�\�I"));
	actions.emplace_back(new CharacterJump(luig, { 0.0f, 500.0f, 0.0f }));
	actions.emplace_back(new LuigDialogEvent(luig, "�I�H"));
	actions.emplace_back(new CharacterTurn(luig, { -1.0f, 0.0f, 0.0f }));
	actions.emplace_back(new DialogHide);
	actions.emplace_back(new CharacterVisible(mario, true));
	actions.emplace_back(new CharacterMove(mario, { 400.0f, 31.0f, -72.131f }));

	actions.emplace_back(new LuigDialogEvent(luig, "���o���܁I�H"));
	actions.emplace_back(new MarioDialogEvent(mario, "���C�I��ς��I�Ƃ̑O�Ƀ����X�^�[���I"));
	actions.emplace_back(new LuigDialogEvent(luig, "�W�J�����߂��܂��񂩁I�H"));
	actions.emplace_back(new MarioDialogEvent(mario, "���ߐ؂����O������X�g�[���[�����킯���Ȃ�����I"));
	actions.emplace_back(new LuigDialogEvent(luig, "���^�^�^�^�^...."));
	actions.emplace_back(new MarioDialogEvent(mario, "�N����ꃕ����RPG��낤�Ƃ��郄�c�́I"));
	actions.emplace_back(new LuigDialogEvent(luig, "��҂̎��Ǝ�������Ȃ��ł���..."));
	actions.emplace_back(new MarioDialogEvent(mario, "�Ƃɂ������������X�^�[�|���ɍs������I�����ꂵ���Ȃ��񂾂���I(�Q�[���I��)"));
	actions.emplace_back(new LuigDialogEvent(luig, "�`���[�g���A���́H"));
	actions.emplace_back(new MarioDialogEvent(mario, "�Ȃ���I�܂�܃}���ZRPG�ƌ������番���邾�낤�I"));
	actions.emplace_back(new LuigDialogEvent(luig, "����ȓK����..."));
	actions.emplace_back(new MarioDialogEvent(mario, "���Ȃ烋�C������Ă�B"));
	actions.emplace_back(new LuigDialogEvent(luig, "������..."));
	actions.emplace_back(new LuigDialogEvent(luig, "�����L�[�͈ړ��AA�AB�{�^���̓W�����v�AShift�Ō��"));
	actions.emplace_back(new MarioDialogEvent(mario, "�}���Z RPG�ƈꏏ�����"));
	actions.emplace_back(new LuigDialogEvent(luig, "���o���܂��邳���ł��B"));
	actions.emplace_back(new CharacterJump(mario, { 0.0f, 500.0f, 0.0f }));
	actions.emplace_back(new MarioDialogEvent(mario, "�����K�v�قǃR���e���c�����ĂȂ���������s�����I"));
	actions.emplace_back(new LuigDialogEvent(luig, "���傤���Ȃ��ł���..."));

	actions.emplace_back(new DialogHide);
	actions.emplace_back(new FlagEvent("flags_opening_movie", true));
}

void EventOpening::Update(float elapsedTime)
{
	EventScene::Update(elapsedTime);

	// �L�����X�V
	mario->Update(elapsedTime);
	luig->Update(elapsedTime);
}