#pragma once

#include <memory>
#include <unordered_map>
#include "StateMachine.h"
#include "Character.h"
#include "GameData.h"

class Enemy;

static const uint32_t Input_Up = (1 << 0);			// ��
static const uint32_t Input_Down = (1 << 1);		// ��
static const uint32_t Input_Left = (1 << 2);		// ��
static const uint32_t Input_Right = (1 << 3);		// �E
static const uint32_t Input_Jump = (1 << 4);		// �W�����v
static const uint32_t Input_Dodge = (1 << 5);		// ���
static const uint32_t Input_Attack_N = (1 << 6);	// ��ʍU��
static const uint32_t Input_Attack_S = (1 << 7);	// ����U��
static const uint32_t Input_Skill_1 = (1 << 8);		// �X�L��1
static const uint32_t Input_Skill_2 = (1 << 9);		// �X�L��2
static const uint32_t Input_Skill_3 = (1 << 10);	// �X�L��3
static const uint32_t Input_Skill_4 = (1 << 11);	// �X�L��4
static const uint32_t Input_Menu = (1 << 12);		// ���j���[

static const uint32_t Input_R_Attack_N = (1 << 13);	// ��ʍU�������[�X
static const uint32_t Input_R_Attack_S = (1 << 14);	// ����U�������[�X
static const uint32_t Input_R_Skill_1 = (1 << 15);	// �X�L��1�����[�X
static const uint32_t Input_R_Skill_2 = (1 << 16);	// �X�L��2�����[�X
static const uint32_t Input_R_Skill_3 = (1 << 17);	// �X�L��3�����[�X
static const uint32_t Input_R_Skill_4 = (1 << 18);	// �X�L��4�����[�X

enum PLAYER_CLASS
{
	Null = -1,
	KNIGHT = 0,
	BARBARIAN,
	ROGUE,

	End,
	MAGE,
};

struct PLAYER_DATA
{
	int client_id = -1;
	DirectX::XMFLOAT3 position = {};
	DirectX::XMFLOAT3 velocity = {};
	DirectX::XMFLOAT3 target = {};
	DirectX::XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
	float angle = 0.0f;
	int state = -1;
	int subState = -1;
	int hp = 1;
	int maxHp = 1;
	bool connected = true;
};

class Player : public Character
{
public:
	Player(const char* filename, float scaling = 1.0f);
	~Player();

	virtual void Update(float elapsedTime) override;
	void Render(const RenderContext& rc) override;

	void Jump();
	void InputMove(float elapsedTime);

	DirectX::XMFLOAT2 GetInputDirection();
	// ���͊Ǘ�
	void UpdateInput();						// �L�[����
	bool InputJump() { return (input & Input_Jump); }
	bool InputDodge();
	bool InputAttackNormal() { return (input & Input_Attack_N) > 0; }
	bool InputAttackSpecial() { return (input & Input_Attack_S) > 0; }
	bool InputSkill1() { return (input & Input_Skill_1) > 0; }
	bool InputSkill2() { return (input & Input_Skill_2) > 0; }
	bool InputSkill3() { return (input & Input_Skill_3) > 0; }
	bool InputSkill4() { return (input & Input_Skill_4) > 0; }
	bool ReleaseAttackNormal() { return (input & Input_R_Attack_N) > 0; }
	bool ReleaseAttackSpecial() { return (input & Input_R_Attack_S) > 0; }
	bool ReleaseSkill1() { return (input & Input_R_Skill_1) > 0; }
	bool ReleaseSkill2() { return (input & Input_R_Skill_2) > 0; }
	bool ReleaseSkill3() { return (input & Input_R_Skill_3) > 0; }
	bool ReleaseSkill4() { return (input & Input_R_Skill_4) > 0; }

	bool IsMove() { return velocity.x != 0.0f || velocity.z != 0.0f; }
	bool IsFall() { return velocity.y < -2.0f; }

	void FaceToCamera();
	void TurnByInput();

	void ImportData(PLAYER_DATA data);
	void ExportData(PLAYER_DATA& data);

	int GetClientId() { return client_id; }
	void SetClientId(int id) { client_id = id; }
	int GetClassType() { return type; }

	float GetTurnSpeed() { return turnSpeed; }
	void SetTurnSpeed(float turnSpeed) { this->turnSpeed = turnSpeed; }

	void SetName(const char* name) { strcpy_s(this->name, 64, name); };

	// MP�Ǘ�
	void RecoverMp(float elapsedTime);
	void ModifyMp(float mp);
	float GetMp() { return mp; }
	float GetMaxMp() { return maxMp; }

	// �X�L���^�C�}�[
	float GetSkillTimerTime(int idx);
	float GetSkillTimerRate(int idx);
	void ResetSkillTimer(int idx);

	// �X�L��
	void SkillCost(int idx);

	void StopMove() { velocity.x = velocity.z = 0.0f; }
	void StopFall() { velocity.y = -gravity * T_TIMER.Delta() * 60.0f; }

	float GetMpCost(int idx);

	// ���@����
	bool IsPlayer() { return GAME_DATA.GetClientId() == client_id; };

	virtual void OnDamage(const HitResult& hit, int damage);

	DirectX::XMFLOAT3 GetTarget() { return target; }
	virtual DirectX::XMFLOAT3 GetShotPosition() { return position + DirectX::XMFLOAT3{ 0, height * 0.5f, 0 }; }
	//virtual DirectX::XMFLOAT3 GetShotAngle() { return angle; }

	int GetState() { return stateMachine->GetStateIndex(); }
	StateMachine<Player>* GetStateMachine() { return stateMachine; }

	Collider* GetAttackCollider(int idx) { return attackColliders[idx]; }
	std::unordered_map<int, Collider*> GetAttackColliders() { return attackColliders; }
	void EnableAttackColliders(bool enable = true) { for (const std::pair<int, Collider*>& collider : attackColliders) collider.second->SetEnable(enable); }

	static DirectX::XMFLOAT4 GetColorSet(int idx) { return Player::colorSet[idx]; }
protected:
	void RegisterCommonState();
	void UpdateTarget();													// ���@�p�A�C���ڕW�X�V
	virtual void UpdateColliders() override;								// �Փ˔���̍X�V

	void UpdateSkillTimers(float elapsedTime);								// �X�L���^�C�}�[

	bool CollisionVsEnemies(
		Collider* collider,
		int damage,
		bool power = false,
		float force = 0.0f,
		int effectIdx = -1,
		float effectScale = 1.0f
	); // �ėp �G�Ƃ̔���
public:
	// KayKit Adventurers
	enum Animation
	{
		OneHanded_Melee_Attack_Chop,
		OneHanded_Melee_Attack_Slice_Diagonal,
		OneHanded_Melee_Attack_Slice_Horizontal,
		OneHanded_Melee_Attack_Stab,
		OneHanded_Ranged_Aiming,
		OneHanded_Ranged_Reload,
		OneHanded_Ranged_Shoot,
		OneHanded_Ranged_Shooting,
		TwoHanded_Melee_Attack_Chop,
		TwoHanded_Melee_Attack_Slice,
		TwoHanded_Melee_Attack_Spin,
		TwoHanded_Melee_Attack_Spinning,
		TwoHanded_Melee_Attack_Stab,
		TwoHanded_Melee_Idle,
		TwoHanded_Ranged_Aiming,
		TwoHanded_Ranged_Reload,
		TwoHanded_Ranged_Shoot,
		TwoHanded_Ranged_Shooting,
		Block,
		Block_Attack,
		Block_Hit,
		Blocking,
		Cheer,
		Death_A,
		Death_A_Pose,
		Death_B,
		Death_B_Pose,
		Dodge_Backward,
		Dodge_Forward,
		Dodge_Left,
		Dodge_Right,
		Dualwield_Melee_Attack_Chop,
		Dualwield_Melee_Attack_Slice,
		Dualwield_Melee_Attack_Stab,
		Hit_A,
		Hit_B,
		Idle,
		Interact,
		Jump_Full_Long,
		Jump_Full_Short,
		Jump_Idle,
		Jump_Land,
		Jump_Start,
		Lie_Down,
		Lie_Idle,
		Lie_Pose,
		Lie_StandUp,
		Pickup,
		Running_A,
		Running_B,
		Running_Strafe_Left,
		Running_Strafe_Right,
		Sit_Chair_Down,
		Sit_Chair_Idle,
		Sit_Chair_Pose,
		Sit_Chair_StandUp,
		Sit_Floor_Down,
		Sit_Floor_Idle,
		Sit_Floor_Pose,
		Sit_Floor_StandUp,
		Spellcast_Charge,
		Spellcast_Long,
		Spellcast_Raise,
		Spellcast_Shoot,
		T_POSE,
		Throw,
		Unarmed_Idle,
		Unarmed_Melee_Attack_Kick,
		Unarmed_Melee_Attack_Punch_A,
		Unarmed_Melee_Attack_Punch_B,
		Unarmed_Pose,
		Use_Item,
		Walking_A,
		Walking_B,
		Walking_Backward,
		Walking_C
	};

	enum class State
	{
		Idle,
		Move,
		Jump,
		Fall,
		Land,
		Dodge,
		Hurt,
		Death,

		AttackNormal,
		AttackSpecial,
		Skill_1,
		Skill_2,
		Skill_4,
		Skill_3,

		Waiting = 998,	// �ҋ@
		Ready = 999,	// �ҋ@ (��������)
	};

	enum COLOR_PATTERN {
		DEFAULT,
		RED,
		GREEN,
		BLUE,
		END
	};
protected:
	uint32_t input = 0;						// �L�[����
	DirectX::XMFLOAT2 inputDirection = {};	// �ړ�����
	DirectX::XMFLOAT3 target = {};			// �A�C���ڕW

	int client_id = -1;
	float mp = 100.0f;
	float maxMp = 100.0f;
	float mpRecoverRate = 8.0f; // ���b�񕜗�
	PLAYER_CLASS type = PLAYER_CLASS::Null;
	int atk = 10;
	char name[64];

	float moveSpeed = 0.0f;
	float turnSpeed = 0.0f;
	float jumpSpeed = 0.0f;
	float dodgeSpeed = 0.0f;

	// MP����
	std::unordered_map<int, float> mpCost;

	// �X�L���N�[���^�C��
	struct SkillTimer {
		float currentTimer;
		float time;
		SkillTimer(float time = 0.0f)
		{
			this->time = time;
			this->currentTimer = 0.0f;
		}
	};
	std::unordered_map<int, SkillTimer> skillTimer;
protected:

	static inline DirectX::XMFLOAT4 colorSet[COLOR_PATTERN::END] = {
		{ 1.0f, 1.0f, 1.0f, 1.0f }, // DEFAULT
		{ 1.0f, 0.7f, 0.7f, 1.0f }, // RED
		{ 0.7f, 1.0f, 0.7f, 1.0f }, // GREEN,
		{ 0.7f, 0.7f, 1.0f, 1.0f }, // BLUE
	};

	StateMachine<Player>* stateMachine;

	std::unordered_map<int, Collider*> attackColliders; // �U������
};
