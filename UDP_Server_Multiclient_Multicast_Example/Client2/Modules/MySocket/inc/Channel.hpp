#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Socket.hpp"

enum class ChannelStatusType
{
    CHANNEL_ON, /* One-to-one communication */
    CHANNEL_OFF /* One-to-many communication */
};
// Abstract Class: Channel
class Channel
{
protected:
    Socket *channelSocket;                                            // Pointer to Socket object (either TCPSocket or UDPSocket)
    ChannelStatusType channelStatus = ChannelStatusType::CHANNEL_OFF; /* Connection status */
public:
    explicit Channel(Socket *socket) : channelSocket(socket) {}

    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void send(const std::string &message) = 0;
    virtual std::string receive() = 0;

    virtual ~Channel() = default;
};

#endif