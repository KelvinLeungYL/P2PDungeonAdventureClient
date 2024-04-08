#include "Character.h"
#include "Mathf.h"
#include "MapTileManager.h"

#define POS_ERROR 0.0001f  // �덷����

void Character::Move(float vx, float vz, float speed) {
	// �ړ�
	moveVecX = vx;
	moveVecZ = vz;

	maxMoveSpeed = speed;
}
void Character::Turn(float elapsedTime, float vx, float vz, float speed) {
	speed *= elapsedTime;

	float vecLength = vx * vx + vz * vz;
	// �i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v�Ȃ�
	if (vecLength < POS_ERROR) return;

	vecLength = sqrtf(vecLength);

	// �i�s�x�N�g����P�ʃx�N�g����
	vx /= vecLength;
	vz /= vecLength;

	// ���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.y);
	float frontZ = cosf(angle.y);

	// ���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (frontZ * vx) - (frontX * vz);

	float dot = (frontX * vx) + (frontZ * vz);
	float rot = 1.0f - dot;
	if (rot > speed) {
		rot = speed;
	}

	// 2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E���肪�s����
	// ���E������s�����Ƃɂ���č��E��]��I������

	if (cross > 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}

	while (angle.y > DirectX::XM_PI) angle.y -= DirectX::XM_2PI;
	while (angle.y < -DirectX::XM_PI) angle.y += DirectX::XM_2PI;
}

bool Character::MoveTo(float elapsedtime, const DirectX::XMFLOAT3& target, float moveSpeed, float turnSpeed)
{
	// Horizon
	float vx = target.x - position.x;
	float vz = target.z - position.z;

	if (vx * vx + vz * vz < moveSpeed * elapsedtime * moveSpeed * elapsedtime) return true;

	Move(vx, vz, moveSpeed);
	Turn(elapsedtime, vx, vz, turnSpeed);

	return false;
}

void Character::Jump(float speed)
{
	velocity.y = speed;
	isGround = false;
}

void Character::UpdateVelocity(float elapsedTime)
{
	// �o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	// �������͍X�V����
	UpdateVerticalVelocity(elapsedFrame);
	// �������͍X�V����
	UpdateHorizontalVelocity(elapsedFrame);

	// �����ړ��X�V����
	UpdateVerticalMove(elapsedTime);
	// �����ړ��X�V����
	UpdateHorizontalMove(elapsedTime);

	// �Փ˔���X�V
	UpdateColliders();
}

void Character::UpdateVerticalVelocity(float elapsedFrame)
{
	velocity.y += gravity * elapsedFrame;
}
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
	if (length > 0.0f)
	{
		// ���C��
		float friction = this->friction * elapsedFrame;
		if (!isGround) friction *= airControl;

		if (length > friction)
		{
			velocity.x -= friction * velocity.x / length;
			velocity.z -= friction * velocity.z / length;
		}
		else
		{
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}
	}
	if (length <= maxMoveSpeed)
	{
		float moveVecLength = sqrtf(moveVecX * moveVecX + moveVecZ * moveVecZ);
		if (moveVecLength > 0.0f)
		{
			// ������
			float acceleration = this->acceleration * elapsedFrame;

			// �ړ��x�N�g���ɂ���������
			velocity.x += acceleration * moveVecX / moveVecLength;
			velocity.z += acceleration * moveVecZ / moveVecLength;

			float length = sqrtf(velocity.x * velocity.x + velocity.z * velocity.z);
			if (length > maxMoveSpeed)
			{
				velocity.x = maxMoveSpeed * velocity.x / length;
				velocity.z = maxMoveSpeed * velocity.z / length;
			}
		}
	}
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

void Character::UpdateVerticalMove(float elapsedTime)
{
	// ���������̈ړ���
	float my = velocity.y * elapsedTime;

	// �L�����N�^�[��Y�������ƂȂ�@���x�N�g��
	DirectX::XMFLOAT3 normal = { 0.0f, 1.0f, 0.0f };

	// ������
	if (my < 0.0f)
	{
		// ���C�̊J�n�ʒu�͑�����菭����
		DirectX::XMFLOAT3 start = { position.x, position.y + stepOffset, position.z };
		// ���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x, position.y + my, position.z };
		// ���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		if (MAPTILES.RayCast(start, end, hit))
		{
			// �n�ʂɐڒn���Ă���
			position = hit.position;
			// ��]
			angle.x += hit.rotation.x;
			angle.y += hit.rotation.y;
			angle.z += hit.rotation.z;

			normal = hit.normal;

			// ���n����
			if (!isGround)
			{
				OnLanding();
			}
			isGround = true;
			velocity.y = -1.0f;
		}
		else
		{
			// �󒆂ɕ����Ă���
			position.y += my;
			isGround = false;
		}
	}
	// �㏸��
	else if (my > 0.0f)
	{// ���C�̊J�n�ʒu�͓�������菭����
		DirectX::XMFLOAT3 start = { position.x, position.y + height - stepOffset, position.z };
		// ���C�̏I�_�ʒu�͈ړ���̈ʒu
		DirectX::XMFLOAT3 end = { position.x, position.y + height + my, position.z };
		// ���C�L���X�g�ɂ��n�ʔ���
		HitResult hit;
		if (MAPTILES.RayCast(start, end, hit))
		{
			// �n�ʂɐڒn���Ă���
			position = hit.position;
			// ��]
			angle.x += hit.rotation.x;
			angle.y += hit.rotation.y;
			angle.z += hit.rotation.z;

			normal = hit.normal;

			velocity.y = 0.0f;
		}
		else {
			position.y += my;
		}
	}

	// �n�ʂ̌����ɉ����悤��XZ����]
	{
		// Y�����@���x�N�g�������Ɍ����I�C���[�p��]���Z�o����
		//angle.x = Mathf::Lerp(angle.x, atan2f(normal.z, normal.y), 0.2f);
		//angle.z = Mathf::Lerp(angle.z, -atan2f(normal.x, normal.y), 0.2f);
	}
}
void Character::UpdateHorizontalMove(float elapsedTime)
{
	// �������͗ʌv�Z
	float velocityLengthXZ = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
	if (velocityLengthXZ > 0.0f)
	{
		// �����ړ��n
		float mx = velocity.x * elapsedTime;
		float mz = velocity.z * elapsedTime;

		// �}�b�v
		if (collider != nullptr) {
			HitResult hit;
			Collider* col = collider.get();
			for (ModelObject*& map : MAPTILES.GetAll())
			{
				DirectX::XMFLOAT3 direction = { mx, 0, mz };
				if (map->GetCollider()->Collision(col, direction, hit))
				{
					hit.normal.y = 0.0f;
					position += hit.normal * hit.distance;
				}
				else
				{
					position.x += mx;
					position.z += mz;
				}
			}
		}
		else
		{
			// ���C�L���X�g
			DirectX::XMFLOAT3 start;
			start = position;
			start.y += stepOffset;

			DirectX::XMFLOAT3 end = DirectX::XMFLOAT3(start.x + mx, start.y, start.z + mz);
			start.x -= (mx * 0.01f);
			start.z -= (mz * 0.01f);

			HitResult hit;
			if (MAPTILES.RayCast(start, end, hit))
			{
				// �ǂ���̃x�N�g��
				DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
				DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
				DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);

				// �ǂ̖@��
				DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

				// ���˃x�N�g����@���Ɏˉe
				DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(DirectX::XMVectorNegate(Vec), Normal);
				Dot = DirectX::XMVectorScale(Dot, 1.1f); // �ǂ߂荞�ݑ΍�

				// �␳�ʒu�̌v�Z
				DirectX::XMVECTOR CorrectPosition = DirectX::XMVectorMultiplyAdd(Normal, Dot, End);
				DirectX::XMFLOAT3 correctPosition;
				DirectX::XMStoreFloat3(&correctPosition, CorrectPosition);

				// �ǎC������փ��C�L���X�g
				HitResult hit2;
				if (!MAPTILES.RayCast(start, correctPosition, hit2))
				{
					// �ǎC������ŕǂɓ���������␳�ʒu�Ɉړ�
					position.x = correctPosition.x;
					position.z = correctPosition.z;
				}
				else
				{
					position.x = hit2.position.x;
					position.z = hit2.position.z;
				}
				if (!isWall)
				{
					OnWall();
				}
				isWall = true;
			}
			else
			{
				isWall = false;
				position.x += mx;
				position.z += mz;
			}
		}
	}
}

void Character::ModifyHp(int hp)
{
	this->hp += hp;
	if (this->hp > maxHp) this->hp = maxHp;
	if (this->hp < 0) this->hp = 0;
}

void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

void Character::Update(float elapsedTime)
{
	if (hurtCoolTime > 0.0f) hurtCoolTime -= elapsedTime;

	UpdateVelocity(elapsedTime);			// �ړ��X�V
	UpdateModel(elapsedTime);
}

void Character::UpdateModel(float elapsedTime)
{
	ModelObject::Update(elapsedTime);
}

void Character::Render(const RenderContext& rc)
{
	ModelObject::Render(rc);
}