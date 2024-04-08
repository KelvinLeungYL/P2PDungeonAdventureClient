#pragma once

class Network
{
public:
	Network() = default;
	~Network() = default;

	// ������
	static bool Initialize();

	// �I����
	static void Finalize();

	// IP�A�h���X�擾
	static bool GetIpAddress(char* buffer, size_t size);
	static void addrToChar(struct addrinfo*& addr, char*& buffer, size_t size);
};