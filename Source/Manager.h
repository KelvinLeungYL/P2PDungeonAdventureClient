#pragma once

#include <vector>

template<typename T>
class Manager
{
public:
	// �A�C�e����o�^����
	virtual T* Register(T* item)
	{
		items.emplace_back(item);
		return item;
	}

	// �A�C�e���̓o�^����������
	virtual void Remove(T* item)
	{
		typename std::vector<T*>::iterator it = std::find(items.begin(), items.end(), item);
		if (it != items.end())
		{
			items.erase(it);
			delete item;
		}
	}

	// �o�^�ς݂̃A�C�e����S�폜����
	virtual void Clear()
	{
		for (T* item : items)
		{
			delete item;
		}
		items.clear();
	}

	T* Replace(int index, T* item)
	{
		delete items.at(index);
		items.at(index) = item;
		return item;
	}

	T* get(int index) { return items.at(index); }

	int Count() { return static_cast<int>(items.size()); }

	std::vector<T*>& GetAll() { return items; }
protected:
	std::vector<T*> items;
};
