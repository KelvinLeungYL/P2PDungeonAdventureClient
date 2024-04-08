#pragma once

//#include <vector>
#include <unordered_map>
#include "BaseState.h"

template <typename T>
class StateMachine
{
public:
	// �R���X�g���N�^
	StateMachine() {}
	// �f�X�g���N�^
	~StateMachine();
	// �X�V����
	void Update(float elapsedTime);
	// �X�e�[�g�Z�b�g
	void SetState(int setstate);
	// �X�e�[�g�ύX
	void ChangeState(int newState);
	// �X�e�[�g�o�^
	void RegisterState(int id, HierarchicalState<T>* state);
	// ���݂̃X�e�[�g�ԍ��擾
	int GetStateIndex();

	// �Q�w�ڃX�e�[�g�ύX
	void ChangeSubState(int newState);
	// �Q�w�ڃX�e�[�g�o�^
	void RegisterSubState(int index, int subIndex, State<T>* subState);
	// �X�e�[�g�擾
	HierarchicalState<T>* GetState() { return currentState; }
private:
	// ���݂̃X�e�[�g
	HierarchicalState<T>* currentState = nullptr;
	// �e�X�e�[�g��ێ�����z��
	std::unordered_map<int, HierarchicalState<T>*> statePool;
};