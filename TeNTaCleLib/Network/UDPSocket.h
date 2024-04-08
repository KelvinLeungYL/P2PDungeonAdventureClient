#pragma once

#include <unordered_map>
#include "Socket.h"

/*
* �T�[�o�[�\�P�b�g
* �N���C�A���g�Ǘ������A�S���󂯓����
*/

class UDPServerSocket : public Socket
{
public:
	UDPServerSocket() = default;
	~UDPServerSocket() override = default;

	bool Connect(const char* address, const char* port) override;

	int Send(int client_id, const void* data, size_t size); // ���M
	int Receive(void* buffer, size_t size, struct sockaddr_in& client_addr); // ��M
	int SendAll(const void* data, size_t size);
	void Remove(int client_id) { if (clients.find(client_id) != clients.end()) clients.erase(client_id); };

	std::unordered_map<int, struct sockaddr_in>& GetClients() { return clients; }
	void Register(int id, struct sockaddr_in addr);
private:
	std::unordered_map<int, struct sockaddr_in> clients;
};

class UDPClientSocket : public Socket
{
public:
	UDPClientSocket() = default;
	~UDPClientSocket() override = default;

	bool Connect(const char* address, const char* port) override;

	int Send(const void* data, size_t size); // ���M
	int Receive(void* buffer, size_t size); // ��M
};
