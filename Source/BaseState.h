#pragma once

#include <unordered_map>

template<typename T>
class State
{
public:
	State(T* owner) : owner(owner) {}
	virtual ~State() = default;

	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsedTime) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void Exit() = 0;

protected:
	T* owner;
};

template<typename T>
class HierarchicalState : public State<T>
{
public:
	// �R���X�g���N�^
	HierarchicalState(T* owner) : State<T>(owner) {};
	virtual ~HierarchicalState() {}
	// �S�Čp����Ŏ���������K�v�����邽�ߏ������z�֐��Ŏ���
	// �X�e�[�g�ɓ��������̃��\�b�h
	virtual void Enter() = 0;
	// �X�e�[�g�Ŏ��s���郁�\�b�h
	virtual void Execute(float elapsedTime) = 0;
	// �X�e�[�g����o�Ă����Ƃ��̃��\�b�h
	virtual void Exit() = 0;
	// �T�u�X�e�[�g�o�^
	virtual void SetSubState(int newState)
	{
		if (statePool.find(newState) == statePool.end()) return;
		subState = statePool[newState];
		subState->Enter();
	}
	// �T�u�X�e�[�g�ύX
	virtual void ChangeSubState(int newState)
	{
		if (statePool.find(newState) == statePool.end()) return;
		// ���݂̃X�e�[�g��Exit�֐������s�A�V�����X�e�[�g���Z�b�g�A�V�����X�e�[�g��Enter�֐����Ăяo���B
		subState->Exit();

		SetSubState(newState);
	}
	// �T�u�X�e�[�g�o�^
	virtual void RegisterSubState(int id, State<T>* state)
	{
		if (statePool.find(id) == statePool.end()) delete statePool[id];
		// �e�X�e�[�g�o�^
		statePool[id] = state;
	}
	// �T�u�X�e�[�g�擾
	virtual State<T>* GetSubState() { return subState; }
	// �T�u�X�e�[�g�ԍ��擾
	virtual int GetSubStateIndex() {
		for (std::pair<int, State<T>*> state : statePool)
		{
			if (state.second == subState)
			{
				// i�ԍ��ڂ̃X�e�[�g�����^�[��
				return state.first;
			}
		}

		// �X�e�[�g��������Ȃ�������
		return -1;
	};
protected:
	std::unordered_map<int, State<T>*> statePool;
	State<T>* subState = nullptr;
};