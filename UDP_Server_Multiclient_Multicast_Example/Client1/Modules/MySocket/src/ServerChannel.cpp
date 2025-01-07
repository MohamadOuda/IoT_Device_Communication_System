#include "ServerChannel.hpp"

ServerChannel::ServerChannel(Socket *socket, int a_port, const std::string a_ip ) : Channel(socket), port(a_port), ip(a_ip), SocketToClient(nullptr) {}

void ServerChannel::start() 
{
    channelSocket->bind(ip, port);
    channelSocket->listen();
    SocketToClient = channelSocket->accept();
    channelStatus = ChannelStatusType::CHANNEL_ON;
}

void ServerChannel::send(const std::string &message) 
{
    if (SocketToClient != nullptr)
    {
        SocketToClient->send(message);
    }
    else
    {
        channelSocket->send(message);
    }
}

std::string ServerChannel::receive() 
{
    if (SocketToClient != nullptr)
    {
        return (SocketToClient->receive());
    }
    else
    {
        return (channelSocket->receive());
    }
}

std::string ServerChannel::getClientIP() const 
{
    /*
     * Checks if a client is connected through `SocketToClient`. If a client is connected,
     * it converts the client's IP address (stored in `SocketToClient->getAddress()->sin_addr`)
     *  into a string representation using `inet_ntoa` and returns it.
     *
     * If no client is connected (`SocketToClient` is `nullptr`),
     * it returns the string `"No client connected"`.
     */
    if (SocketToClient != nullptr)
    {
        return std::string(inet_ntoa(SocketToClient->getAddress()->sin_addr));
    }
    else
    {
        return "No client connected";
    }
}

void ServerChannel::stop() 
{
    if (channelStatus == ChannelStatusType::CHANNEL_ON)
    {
        if (SocketToClient != nullptr)
        {
            SocketToClient->shutdown();
            delete SocketToClient; /*the parantheses don't affect the functionality of delete and can be deleted for readability */
            SocketToClient = nullptr;
        }

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

// Destructor for ServerChannel
ServerChannel::~ServerChannel()
{
    stop(); // Ensure all shutdown procedures are handled
}
