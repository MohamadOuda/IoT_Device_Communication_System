#ifndef SERVERCHANNEL_HPP
#define SERVERCHANNEL_HPP

#include "Channel.hpp"

class ServerChannel : public Channel
{

private:
    int port; /** Data member to store the port*/

    const std::string ip; /** Data member to store the ip*/
    /*
     * Only used to specify multicast IP for unicast the server isn't bound to any ip
     */

    Socket *SocketToClient; /* Store socket to client for TCP to send to*/

public:
    explicit ServerChannel(Socket *socket, int a_port, const std::string a_ip = "");
    void start() override;
    void send(const std::string &message) override;
    std::string receive();
    std::string getClientIP() const;
    void stop() override;
    // Destructor for ServerChannel
    ~ServerChannel();
};


#endif