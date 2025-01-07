#ifndef TCPSOCKET_HPP
#define TCPSOCKET_HPP

#include "Socket.hpp"

class TCPSocket : public Socket
{
private:
    int sock; // Socket file descriptor
    struct sockaddr_in address; // Structure for address details

    explicit TCPSocket(int a_clientSock, struct sockaddr_in a_address);

public:
    TCPSocket();
    const struct sockaddr_in* getAddress() const override;
    void connect(const std::string &a_ip, int a_port) override;
    void bind(const std::string &a_ip, int a_port) override;
    void listen(int backlog = 5) override;
    Socket* accept() override;
    void send(const std::string &message) override;
    std::string receive() override;
    void shutdown() override;
};

#endif // TCPSOCKET_HPP
