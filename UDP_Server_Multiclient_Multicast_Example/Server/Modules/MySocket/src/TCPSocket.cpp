#include "TCPSocket.hpp"

TCPSocket::TCPSocket()
{
    /* socket(AF_INET, SOCK_STREAM, 0) creates a TCP socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        std::cerr << "Socket creation failed!" << std::endl;
    }
}

TCPSocket::TCPSocket(int a_clientSock, struct sockaddr_in a_address) : sock(a_clientSock), address(a_address)
{
    /**
     * ! The explicit keyword:
     * * Prevents unintended implicit conversions, making code clearer and avoiding bugs.
     * * Requires explicit calls to constructors and conversion operators, enhancing control over object creation and conversion.
     */
}

const struct sockaddr_in* TCPSocket::getAddress() const
{
    return &address;
}
void TCPSocket::connect(const std::string &a_ip, int a_port) 
{
    /*
     ! 1- Defining the Server Address:
     * sockaddr_in is a structure that specifies address details:
     * sin_family: Must match the socket’s address family,
     *             so it’s set to AF_INET for IPv4.
     * sin_port: Specifies the port number. htons(a_port) converts a_port
     *           to network byte order (big-endian).
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
    ! 3- Attempting to Connect to the Server:
    * connect attempts to establish a TCP connection to the server:
    *  sock: The socket file descriptor.
    *  (struct sockaddr*)&address: A pointer to the server’s address structure, cast to struct sockaddr*.
    *  sizeof(address): Specifies the size of the address structure.
    *
    *
    *  If connect fails (returns -1), it indicates the server is unreachable or the connection is denied. An error message is printed, and the program exits.
    */

    if (::connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        /**
         *! THROW
         */
        std::cerr << "Connection failed!" << std::endl;
    }
}
void TCPSocket::bind(const std::string &a_ip, int a_port) 
{

    /*
     ! 1 - Binding the Socket to an IP Address and Port
     *  sockaddr_in defines the server address:
     *    sin_family: Set to AF_INET for IPv4.
     *    sin_addr.s_addr = INADDR_ANY: This allows the server to accept connections on any
     *          available network interface (e.g., localhost or any network IP assigned to the server).
     *
     *    sin_port: Sets the port to a_port, converted to network byte order with htons
     *          (host to network short) to ensure compatibility between different systems.
     */

    // int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(a_port);

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
void TCPSocket::listen(int backlog) 
{
    /*
     ! 3 - Putting the Server in a Listening State:
     * listen(sock, 3) puts the server socket into a listening state,
     *  allowing it to queue up to 3 incoming connections.
     *
     * If listen fails (returns -1), the program exits with an error message.
     */

    if (::listen(sock, backlog) < 0)
    {
        /**
         *! THROW
         */
        std::cerr << "Listen failed!" << std::endl;
    }
}

Socket *TCPSocket::accept() 
{
    /*
     ! 4 - Accepting an Incoming Connection:
     * accept waits for an incoming connection. When a client attempts to connect,
     * accept returns a new socket file descriptor (new_socket) specifically for communicating with that client.
     *
     * accept parameters:
     *  sock : The server’s listening socket.
     *  (struct sockaddr*)&address : A pointer to where the client’s address information will be stored
     *     nullptr means we don't care about client info and it will not be saved this is for the sake of simple connectiion
     *  (socklen_t*)&addrlen : A pointer to the size of the client address structure.
     *
     * If accept returns -1, it indicates a failure to accept the connection, often due to client disconnection or a timeout.
     */
    struct sockaddr_in client_address;
    int addrlen = sizeof(client_address);
    int client_sock = ::accept(sock, (struct sockaddr *)&client_address, (socklen_t *)&addrlen);
    if (client_sock < 0)
    {
        /**
         *! THROW
         */
        std::cerr << "Accept failed!" << std::endl;
        return nullptr;
    }
    return new TCPSocket(client_sock, client_address);
}

void TCPSocket::send(const std::string &message) 
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
        ::send(sock, message.c_str(), message.size(), 0);
    }
}

std::string TCPSocket::receive() 
{
    /* Create a vector that will grow as needed*/
    std::vector<char> buffer(1024); /* Initial size of 1024, but can grow as needed*/
    /**
     * ! recv Function
     * * The recv function is specifically designed for receiving data over sockets.
     * * It is part of the socket API in the <sys/socket.h> library.
     *
     * ~ sock: The socket file descriptor.
     * ~ buf: A pointer to the buffer where the received data will be stored.
     * ~ len: The maximum number of bytes to read.
     * ~ flags: This specifies any special options for receiving data (e.g., MSG_PEEK, MSG_WAITALL, etc.). These flags control how the data is retrieved.
     *
     */

    int bytes = ::recv(sock, buffer.data(), buffer.size(), 0);

    /* If more data is received than the buffer can hold, resize the buffer*/
    if (bytes == buffer.size())
    {
        buffer.resize(buffer.size() * 2); /* Double the buffer size for the next read*/
        bytes += ::recv(sock, buffer.data() + bytes, buffer.size() - bytes, 0);
    }

    return std::string(buffer.data(), bytes); /* Construct a string from the received data*/
}
void TCPSocket::shutdown() 
{

    if (sock >= 0)
    {
        /**
         * !::shutdown(sock, SHUT_RDWR);
         * * Purpose: The shutdown function is used to partially or fully close a connection on a socket without immediately releasing the socket file descriptor (sock).
         * * This allows you to control the flow of data termination on the socket.
         *
         * ~ sock: The socket file descriptor you want to shut down.
         * ~ SHUT_RDWR: This flag specifies that the shutdown should affect both reading and writing on the socket.
         *   ^ SHUT_RD: Closes the reading side of the socket. The socket can no longer receive data, but it can still send.
         *   ^ SHUT_WR: Closes the writing side of the socket. The socket can no longer send data, but it can still receive.
         *   ^ SHUT_RDWR: Closes both reading and writing, disallowing both sending and receiving data on the socket.
         *
         * ? After calling shutdown with SHUT_RDWR, the socket is notified that no more data will be sent or received.
         * ? However, the socket remains open until you call close(). The peer (remote side) is also notified that no more data will be exchanged.
         */
        ::shutdown(sock, SHUT_RDWR);

        /**
         * !close(sock);
         * * Purpose: The close function releases the socket file descriptor and frees any resources associated with it.
         */
        close(sock);
        sock = -1;
    }
}
