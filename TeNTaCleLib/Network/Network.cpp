#include "Network.h"

#include <WS2tcpip.h>

// ������
bool Network::Initialize()
{
	// WinSock������
	WSADATA	wsaData;
	WSAStartup(
		MAKEWORD(2, 2), //WORD �^ WinSock �̃o�[�W���� MAKEWORD �}�N�����g�p����
		&wsaData //WSADATA �\���̂ւ̃|�C���^�i�����ɏ��������ꂽ��񂪓���j
	);

	return true;
}

void Network::Finalize()
{
	WSACleanup();
}

// IP�A�h���X�擾
bool Network::GetIpAddress(char* buffer, size_t size)
{
	//�z�X�g�����擾����
	char hostname[256];
	if (gethostname(hostname, sizeof(hostname)) != 0)
	{
		return false;
	}
	//�z�X�g������IP�A�h���X���擾����
	struct addrinfo hints, * res;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_family = AF_INET;
	if (getaddrinfo(hostname, nullptr, &hints, &res) != 0)
	{
		return false;
	}
	struct in_addr addr;
	addr.S_un = ((struct sockaddr_in*)(res->ai_addr))->sin_addr.S_un;

	switch (res->ai_family)
	{
	case AF_INET:
	case AF_INET6:
		addrToChar(res, buffer, size);
		break;
	default:
		return false;
	}
	return true;
}

void Network::addrToChar(struct addrinfo*& addr, char*& buffer, size_t size)
{
	switch (addr->ai_family)
	{
	case AF_INET:
		inet_ntop(addr->ai_family, &((struct sockaddr_in*)addr->ai_addr)->sin_addr, buffer, size);
		break;
	case AF_INET6:
		inet_ntop(addr->ai_family, &((struct sockaddr_in6*)addr->ai_addr)->sin6_addr, buffer, size);
		break;
	}
}