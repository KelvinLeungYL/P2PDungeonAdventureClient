#pragma once
#include "Socket.h"

class TCPServerSocket : public Socket
{
public:
	TCPServerSocket(int max_conn = 5) : max_conn(max_conn) {}
	~TCPServerSocket() override = default;

	bool Connect(const char* address, const char* port) override;

	bool Accept(); // �ڑ�����

	void SetBlocking(bool block = true);

	int Send(int id, const void* data, size_t size); // ���M
	int Receive(int id, void* buffer, size_t size); // ��M

	bool Disconnect() override;
public:
	struct Client
	{
		int client_id = 0;
		SOCKET sock = INVALID_SOCKET;
		struct sockaddr_in addr = {};
	};

private:
	int max_conn;
	std::vector<Client> clients;
public:
	std::vector<Client>& GetClients() { return clients; }
	int GetClientCount() { return static_cast<int>(clients.size()); }
};

class TCPClientSocket : public Socket
{
public:
	TCPClientSocket() = default;
	~TCPClientSocket() override = default;

	bool Connect(const char* address, const char* port) override;

	int Send(const void* data, size_t size); // ���M
	int Receive(void* buffer, size_t size); // ��M
};
