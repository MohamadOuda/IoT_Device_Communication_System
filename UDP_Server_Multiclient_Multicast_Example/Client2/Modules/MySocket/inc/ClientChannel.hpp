#ifndef CLIENTCHANNEL_HPP
#define CLIENTCHANNEL_HPP

#include "Channel.hpp"

// Derived Class: ClientChannel
class ClientChannel : public Channel
{
private:
    int port; /** Data member to store the port*/

    const std::string ip; /** Data member to store the ip*/

public:
    explicit ClientChannel(Socket *a_socket, int a_port, std::string a_ip);
    void start() override;
    void send(const std::string &message) override;
    std::string receive() override;
    void stop() override; 
    // Destructor for ClientChannel
    ~ClientChannel();
};


#endif