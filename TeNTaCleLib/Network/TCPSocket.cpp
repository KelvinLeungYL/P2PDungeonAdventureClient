#include "TCPSocket.h"

bool TCPServerSocket::Connect(const char* address, const char* port)
{
	// �A�h���X���ݒ�
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int iResult = getaddrinfo(address, port, &hints, &addr);
	if (iResult != 0)
	{
		return false;
	}
	// �\�P�b�g�쐬
	sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (sock == INVALID_SOCKET)
	{
		return false;
	}
	// �\�P�b�g�̎󂯓�������֘A�t��
	iResult = bind(sock, addr->ai_addr, static_cast<int>(addr->ai_addrlen));
	if (iResult == SOCKET_ERROR)
	{
		return false;
	}
	// �N���C�A���g�̐ڑ��҂��󂯏���
	iResult = listen(sock, max_conn);
	if (iResult == SOCKET_ERROR)
	{
		return false;
	}

	// �m���u���b�L���O�ݒ�
	SetBlocking(false);

	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	return true;
}

void TCPServerSocket::SetBlocking(bool block)
{
	u_long value = (block) ? 0 : 1;	// 0:�u���b�L���O / 1:�m���u���b�L���O
	ioctlsocket(sock, FIONBIO, &value);
}

// �ڑ�����
bool TCPServerSocket::Accept()
{
	Client client;
	int size = sizeof(sockaddr_in);
	client.client_id = static_cast<int>(clients.size());
	client.sock = accept(sock, reinterpret_cast<sockaddr*>(&client.addr), &size);
	if (client.sock == INVALID_SOCKET)
	{
		return false;
	}
	clients.emplace_back(client);

	std::string log = "New TCP Connection: " + IPtoStr(&client.addr);
	T_CONSOLE.Log(log.c_str());

	return true;
}

// ���M
int TCPServerSocket::Send(int id, const void* data, size_t size)
{
	int iResult = send(clients[id].sock, reinterpret_cast<const char*>(data), static_cast<int>(size), 0);
	if (iResult == SOCKET_ERROR)
	{
		return -1;
	}
	return iResult;
}
// ��M
int TCPServerSocket::Receive(int id, void* buffer, size_t size)
{
	int recvsize = recv(clients[id].sock, reinterpret_cast<char*>(buffer), static_cast<int>(size), 0);
	if (recvsize == SOCKET_ERROR)
	{
		return -1;
	}
	return recvsize;
}

bool TCPServerSocket::Disconnect()
{
	for (Client& client : clients)
	{
		closesocket(client.sock);
	}
	return Socket::Disconnect();
}

bool TCPClientSocket::Connect(const char* address, const char* port)
{
	// �A�h���X���ݒ�
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int iResult = getaddrinfo(address, port, &hints, &addr);
	if (iResult != 0)
	{
		return false;
	}
	// �\�P�b�g�쐬
	sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
	if (sock == INVALID_SOCKET)
	{
		return false;
	}

	// �^�C���A�E�g�ݒ�(10000ms)
	int timeout = 10000;
	int result = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<char*>(&timeout), sizeof(timeout));
	if (result == SOCKET_ERROR)
	{
		return false;
	}

	//	�T�[�o�[�ڑ�
	result = connect(sock, addr->ai_addr, static_cast<int>(addr->ai_addrlen));
	if (result == SOCKET_ERROR)
	{
		return false;
	}

	FD_ZERO(&fds);
	FD_SET(sock, &fds);

	return true;
}

int TCPClientSocket::Send(const void* data, size_t size) // ���M
{
	int iResult = send(sock, reinterpret_cast<const char*>(data), static_cast<int>(size), 0);
	if (iResult == SOCKET_ERROR)
	{
		return -1;
	}
	return iResult;
}

int TCPClientSocket::Receive(void* buffer, size_t size) // ��M
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 0;

	fd_set	fd_work;
	memcpy(&fd_work, &fds, sizeof(fd_set));
	// fds�ɐݒ肳�ꂽ�\�P�b�g���ǂݍ��݉\�ɂȂ�܂ő҂��܂�
	int n = select(0, &fd_work, NULL, NULL, &tv);
	// �^�C���A�E�g�̏ꍇ��select��0��Ԃ��܂�
	if (n <= 0) return -1;

	int iResult = recv(sock, reinterpret_cast<char*>(buffer), static_cast<int>(size), 0);
	if (iResult == SOCKET_ERROR)
	{
		return -1;
	}

	return iResult;
}