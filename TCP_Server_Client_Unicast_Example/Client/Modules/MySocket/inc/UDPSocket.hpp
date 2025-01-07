#ifndef UDPSOCKET_HPP
#define UDPSOCKET_HPP

#include "Socket.hpp"

/*
  ? enum class Advantages:
  * Scoped: Enumerator names are scoped to the enum itself,avoiding name collisions.
  *         e.g: CommunicationType A = CommunicationType::UNICAST;
  * Type-safe: No implicit conversion to integers, preventing accidental misuse.
  * Better Readability & Prevents mixing enums
  *
  * Explicit underlying type: You can specify the storage type
  *         e.g.: enum class A:uint8_t{};
  *
 */
enum class CommunicationType
{
    UNICAST,  /* One-to-one communication */
    MULTICAST /* One-to-many communication */
};

// Derived Class: UDPSocket
class UDPSocket : public Socket
{
private:
    /** @param  sock : Socket File Descriptor. */
    int sock;

    /** @param  address : Structure that specifies address details. */
    struct sockaddr_in address;

    /** @param  client_address : Structure that specifies address details of last client when UDP Server Socket. */
    struct sockaddr_in client_address;

    /** @param  UDPSocketCommunicationType : Specifies unicast(default) or multicast.*/
    CommunicationType UDPSocketCommunicationType;

    /** @param  mreq : Structure used to join the multicast group. */
    struct ip_mreq mreq;

    /** @param  ttl : TTL (Time to Live) for the multicast packet. */
    unsigned char ttl;

public:
    UDPSocket(CommunicationType a_CommunicationType = CommunicationType::UNICAST, unsigned char a_ttl = 1) ;
    const struct sockaddr_in* getAddress() const override;
    void SetTTL(unsigned char a_ttl);
    void JoinMulticast(const std::string &multicast_ip, int multicast_port);
    void connect(const std::string &a_ip, int a_port) override;
    void bind(const std::string &a_ip, int a_port) override;
    void listen(int backlog = 5) override;
    Socket *accept() override;  void send(const std::string &message) override;
    std::string receive() override;
    void LeaveMulticast(void);
    void shutdown() override;
    
};



#endif