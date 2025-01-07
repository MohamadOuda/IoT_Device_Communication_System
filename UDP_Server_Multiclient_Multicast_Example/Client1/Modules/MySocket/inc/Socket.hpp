#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h> // For sockaddr_in and INADDR_ANY
#include <string>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h> // For socket functions and definitions
#include <arpa/inet.h>  // For inet_pton and inet_ntoa
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <vector>

// Abstract Class: Socket
class Socket
{
public:
    virtual const struct sockaddr_in* getAddress() const = 0;
    virtual void connect(const std::string &a_ip, int a_port) = 0;
    virtual void bind(const std::string &a_ip, int a_port) = 0;
    virtual void listen(int backlog =5) = 0;
    virtual Socket* accept() = 0;
    virtual void send(const std::string &message) = 0;
    virtual std::string receive() = 0;
    virtual void shutdown() = 0;
    virtual ~Socket() = default;
};

#endif // SOCKET_HPP