#pragma once

class Console
{
private:
	Console() = default;
	~Console() = default;
public:
	// �C���X�^���X
	static Console& Instance()
	{
		static Console instance;
		return instance;
	}

	void Open();
	void Close();

	void Log(const char* text);

private:
	bool isOpen = false;
};
