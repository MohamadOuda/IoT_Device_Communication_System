#include "UDPSocket.hpp"

UDPSocket::UDPSocket(CommunicationType a_CommunicationType, unsigned char a_ttl) : UDPSocketCommunicationType(a_CommunicationType), ttl(a_ttl)
{
    /*
    ! 1 - Creating the Socket
    * AF_INET: IPv4 addressing.
    * SOCK_DGRAM: Specifies a datagram socket for UDP.
    * 0: Uses the default protocol for UDP.
    *
    * * If socket returns a negative value, it means the socket creation failed.
    * In that case, an error message is printed, and the program exits.
    */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        /**
         *! THROW
         */
        std::cerr << "Socket creation failed!" << std::endl;
    }
    if (UDPSocketCommunicationType == CommunicationType::MULTICAST)
    {
        int reuse = 1;
        if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0)
        {
            /**
             *! THROW
             */
            std::cerr << "Setting SO_REUSEADDR failed" << std::endl;
            close(sock);
        }
    }
}

const struct sockaddr_in *UDPSocket::getAddress() const 
{
    return &address;
}

void UDPSocket::SetTTL(unsigned char a_ttl)
{
    if (UDPSocketCommunicationType == CommunicationType::MULTICAST)
    {
        ttl = a_ttl;
        if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl)) < 0)
        {
            /**
             *! THROW
             */
            std::cerr << "Failed to set TTL" << std::endl;
            close(sock);
        }
    }
    else
    {
        std::cerr << "SetTTL was called for a unicast soccket!" << std::endl;
    }
}

void UDPSocket::JoinMulticast(const std::string &multicast_ip, int multicast_port)
{
    // Convert the IP to network byte order and check if it's a valid multicast address
    in_addr_t multicast_addr = inet_addr(multicast_ip.c_str());
    if (!IN_MULTICAST(ntohl(multicast_addr)))
    {
        /**
         *! THROW
         */
        std::cerr << "Provided IP is not a valid multicast address!" << std::endl;
        close(sock);
        return;
    }

    /*
     ! 1- Defining the Server Address:
     * sockaddr_in is a structure that specifies address details:
     * sin_family: Must match the socket’s address family, so it’s set to AF_INET for IPv4.
     * sin_port: Specifies the port number. htons(multicast_port) converts multicast_port to network byte order.
     * sin_addr.s_addr: INADDR_ANY allows the client to join the multicast group on all available interfaces.
     */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(multicast_port);

    if (::bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        /**
         *! THROW
         */
        std::cerr << "Bind failed" << std::endl;
        close(sock);
        return;
    }

    /*
     ! 2- Joining the Multicast Group:
     * Set up mreq to specify the multicast group and local interface.
     * mreq.imr_multiaddr.s_addr is set to the multicast group IP, converted from the string.
     * mreq.imr_interface.s_addr is set to INADDR_ANY to listen on all interfaces.
     */
    mreq.imr_multiaddr.s_addr = multicast_addr;
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        /**
         *! THROW
         */
        std::cerr << "Failed to join multicast group" << std::endl;
        close(sock);
    }
}

void UDPSocket::connect(const std::string &a_ip, int a_port) 
{
    /*If the socket communication type is multicast call JoinMulticast */
    if (UDPSocketCommunicationType == CommunicationType::MULTICAST)
    {
        JoinMulticast(a_ip, a_port);
        return;
    }
    /*
     ! 1- Defining the Server Address:
     * sockaddr_in is a structure that specifies address details:
     * sin_family: Must match the socket’s address family,
     *             so it’s set to AF_INET for IPv4.
     * sin_port: Specifies the port number. htons(a_port) converts a_port
     *           to network byte order (big-endian).
     *
     ~ Comparison to TCP:
     ~  Both the UDP and TCP clients define the server address similarly.
     ~  However, in UDP, no actual "connection" is established with the server.
     ~  Instead, UDP simply uses this address to direct datagrams to the server.
     */
    address.sin_family = AF_INET;
    address.sin_port = htons(a_port);

    /*
     ! 2- Converting and Setting the IP Address:
     * inet_pton (presentation to network) converts the IP address into
     * a binary format suitable for the socket’s address structure (address.sin_addr).
     ~ The ip.c_str() call is used to convert a C++ std::string to a C-style string (i.e., a const char*).
     ~ This conversion is necessary because many low-level networking functions in C, such as inet_pton, require a const char* rather than a C++ std::string.
     *
     * If inet_pton fails (e.g., if the IP is incorrectly formatted), it returns 0 or -1,
     * indicating an invalid address.
     */
    if (inet_pton(AF_INET, a_ip.c_str(), &address.sin_addr) <= 0)
    {
        /**
         *! THROW
         */
        std::cerr << "Invalid address!" << std::endl;
    }

    /*
     ~Comparison to TCP:
     ~  In the TCP client, connect establishes a session before sending data with send.
     ~  UDP does not use connect. Each sendto sends a datagram to the specified server,
     ~     regardless of any prior communication. UDP is stateless and connectionless,
     ~     meaning each datagram is independent.
     */
}

void UDPSocket::bind(const std::string &a_ip, int a_port) 
{

    /*
     ! 1 - Binding the Socket to an IP Address and Port
     *  sockaddr_in defines the server address:
     *    sin_family: Set to AF_INET for IPv4.
     *
     *    sin_port: Sets the port to a_port, converted to network byte order with htons
     *          (host to network short) to ensure compatibility between different systems.
     */

    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_port = htons(a_port);

    /*
     ? Check if the server is unicast or multicast
     ~  Server is UNICAST
     *    sin_addr.s_addr = INADDR_ANY: This allows the server to accept connections on any
     *          available network interface (e.g., localhost or any network IP assigned to the server).
     *
     ~ Server is MULTICAST
     *    sin_addr.s_addr = inet_addr(a_ip): sets the address to the multicast server address
     *    SetTTL : sets the Time To Live of multicast server packet.
     *    return : As there's no need to bind where The server does not need to bind to a specific local
     *             address because it's simply sending packets to the multicast group.
     *
     */
    if (UDPSocketCommunicationType == CommunicationType::UNICAST)
    {
        address.sin_addr.s_addr = INADDR_ANY;
    }
    else if (UDPSocketCommunicationType == CommunicationType::MULTICAST)
    {
        if (a_ip.empty())
        {
            /**
             *! THROW
             */
            std::cerr << "No Multicast IP was provided!" << std::endl;
        }
        address.sin_addr.s_addr = inet_addr(a_ip.c_str());
        SetTTL(ttl);
        return;
    }
    else
    {
    }

    /*
     * bind associates the server socket with the specified IP address and port.
     * SOCK_STREAM specifies a TCP socket.
     *
     *
     * If bind fails (returns -1), it indicates that the port may already be in use,
     * or the server lacks necessary permissions. An error message is printed, and the program exits.
     */

    if (::bind(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        /**
         *! THROW
         */
        std::cerr << "Bind failed!" << std::endl;
    }
}

void UDPSocket::listen(int backlog ) 
{
    /*
     ! No Listen For UDP
    */
}

Socket *UDPSocket::accept() 
{
    /*
     ! No Accept for UDP
    */

    return nullptr;
}

void UDPSocket::send(const std::string &message) 
{
    /*
     * send(sock, message, strlen(message), 0) sends the message "Hello from client" to the server.
     * The arguments to send are:
     * sock: the socket file descriptor.
     * message: the data to send.
     * strlen(message): the length of the data.
     * 0: no special flags are used.
     * */
    if (sock >= 0)
    {
        ::sendto(sock, message.c_str(), strlen(message.c_str()), 0, (const struct sockaddr *)&client_address, sizeof(client_address));
    }
}

std::string UDPSocket::receive() 
{
    /* Create a vector that will grow as needed*/
    std::vector<char> buffer(1024); /* Initial size of 1024, but can grow as needed*/
    /**
     * ! recvfrom Function (for UDP)
     * * The recvfrom function is used for receiving data on a socket (UDP).
     * * It allows us to get the sender's address, which is essential for UDP communication.
     *
     * ~ sock: The socket file descriptor.
     * ~ buf: A pointer to the buffer where the received data will be stored.
     * ~ len: The maximum number of bytes to read.
     * ~ flags: Any special options for receiving data.
     * ~ src_addr: A pointer to a sockaddr structure to hold the address of the sender.
     * ~ addrlen: A pointer to the size of the address structure.
     *
     */
    socklen_t addrlen = sizeof(client_address);

    int bytes = ::recvfrom(sock, buffer.data(), buffer.size(), 0, (struct sockaddr *)&client_address, &addrlen);

    // Check if an error occurred
    if (bytes < 0)
    {
        std::cerr << "Failed to receive data." << std::endl;
        return "";
    }

    /* If more data is received than the buffer can hold, resize the buffer*/
    if (bytes == buffer.size())
    {
        buffer.resize(buffer.size() * 2); /* Double the buffer size for the next read*/
        /*
         ! recvfrom is called again to receive any remaining data.
         * Parameters:
         * buffer.data() + bytes is a pointer to the start of the newly added portion of the buffer.
         * buffer.data() gives a pointer to the start of the buffer.
         *   + bytes moves this pointer to the position where the initial recvfrom call left off, so new data will be added after the existing data.
         *  buffer.size() - bytes is the remaining space in the buffer after the initial data, providing the maximum number of bytes recvfrom can read into the expanded buffer.
         *
         * additional_bytes stores the number of bytes received in this second call.
         */
        int additional_bytes = ::recvfrom(sock, buffer.data() + bytes, buffer.size() - bytes, 0, (struct sockaddr *)&address, &addrlen);

        /*This is a safeguard to handle cases where the second recvfrom may not receive any additional data.*/
        if (additional_bytes > 0)
        {
            bytes += additional_bytes;
        }
    }

    return std::string(buffer.data(), bytes); /* Construct a string from the received data*/
}

void UDPSocket::LeaveMulticast(void)
{
    setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
}
void UDPSocket::shutdown() 
{
    /*
    ! Checks sock value if =-1 then it was closed already or failed to be created
    */
    if (sock >= 0)
    {
        /*
         ! Leave Multicast should only be exceuted for a UDP Socket client in Multicast
         ~ 1st condition : Checks if the socket is used in multicast
         ~ 2nd condition : (mreq.imr_multiaddr.s_addr != 0) ensure that the LeaveMulticast function is only called for
         ~                 sockets that have actually joined a multicast group(client).
         ? ip_mreq Struct:
         ?   The ip_mreq struct is used to specify the multicast group and local interface for joining a multicast group.
         ?   The imr_multiaddr.s_addr field holds the IP address of the multicast group.
         ?   When a socket joins a multicast group, this field is populated with the IP address of the group.
         ? Checking for Non-Zero Value:
         ?   If mreq.imr_multiaddr.s_addr is zero, it indicates that the socket has not joined any multicast group.
         */
        if ((UDPSocketCommunicationType == CommunicationType::MULTICAST) && (mreq.imr_multiaddr.s_addr != 0))
        {
            LeaveMulticast();
        }
        /**
         * ~ Comparison with TCP
         * ~ Using ::shutdown() with UDP would be redundant because:
         * ~ UDP doesn't require a formal termination of the connection. The concept of "gracefully closing" a socket doesn't apply in the same way it does with TCP.
         * ~ shutdown() would only affect the reading and writing capabilities of the socket, but since UDP doesn't maintain a continuous session, this step is unnecessary.
         */
        /**
         * !close(sock);
         * * Purpose: The close function releases the socket file descriptor and frees any resources associated with it.
         */
        close(sock);
        sock = -1;
    }
}
