#include "ClientChannel.hpp"

 ClientChannel::ClientChannel(Socket *a_socket, int a_port, std::string a_ip) : Channel(a_socket), port(a_port), ip(a_ip) {}

void ClientChannel::start() 
{
    channelSocket->connect(ip, port);
    channelStatus = ChannelStatusType::CHANNEL_ON;
}

void ClientChannel::send(const std::string &message) 
{
    channelSocket->send(message);
}

std::string ClientChannel::receive() 
{
    return (channelSocket->receive());
}

void ClientChannel::stop() 
{
    if (channelStatus == ChannelStatusType::CHANNEL_ON)
    {
        if (channelSocket != nullptr)
        {
            channelSocket->shutdown();
        }
        else
        {
            /**
             *! THROW
             */
            std::cerr << "Socket Deleted Before stopping the server ensure to call server.stop() before deletion" << std::endl;
        }

        channelStatus = ChannelStatusType::CHANNEL_OFF;
    }
}
// Destructor for ClientChannel
ClientChannel::~ClientChannel()
{
    stop(); // Ensure the socket is properly shut down
}
