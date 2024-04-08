#pragma once

#include <set>
#include "Manager.h"

#include "GameObject.h"

template<typename T>
class ObjectManager : public Manager<T>
{
public:
	virtual void Update(float elapsedTime)
	{
		for (T* item : removes)
		{
			typename std::vector<T*>::iterator it = std::find(this->items.begin(), this->items.end(), item);
			if (it != this->items.end())
			{
				this->items.erase(it);
			}
			delete item;
		}
		removes.clear();

		size_t size = this->items.size();
		for (size_t i = 0; i < size; i++)
		{
			if (this->items.size() <= i) break;
			this->items.at(i)->Update(elapsedTime);
		}
	}
	void Render(const RenderContext& rc)
	{
		for (T* item : this->items)
		{
			item->Render(rc);
		}
	}

	void MoveToEnd(T* item)
	{
		typename std::vector<T*>::iterator it = std::find(this->items.begin(), this->items.end(), item);
		if (it != this->items.end())
		{
			this->items.erase(it);
			this->Register(item);
		}
	}
	// �A�C�e���̓o�^����������
	void Remove(T* item)
	{
		removes.insert(item);
	}
	// ���T�C�Y
	void Resize(int size)
	{
		int i = 0;
		for (T* item : this->items)
		{
			i++;
			if (i > size) removes.insert(item);
		}
	}
	// �f�o�b�O���̕\��
	void DrawDebugGUI()
	{
		for (T* item : this->items)
		{
			item->DrawDebugGUI();
		}
	}
	// �f�o�b�O�v���~�e�B�u�̕\��
	void DrawDebugPrimitive()
	{
		for (T* item : this->items)
		{
			item->DrawDebugPrimitive();
		}
	}

protected:
	std::set<T*> removes;
};
