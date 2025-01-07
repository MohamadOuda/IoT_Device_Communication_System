#include "TCPSocket.hpp"
#include "ClientChannel.hpp"


int main()
{
    int port = 8080;  // Port to connect to the server
    std::string serverIp = "127.0.0.1";  // IP of the server (localhost for testing)

    // Create the socket for the client (TCP socket)
    Socket *clientSocket = new TCPSocket();  // Assuming TCPSocket is a subclass of Socket
    ClientChannel clientChannel(clientSocket, port, serverIp);  // Create ClientChannel

    clientChannel.start();  // Connect to the server
    std::cout << "Client: Connected to Server." << std::endl;

    // Send message to the server
    std::string message = "Hello, Server!";
    clientChannel.send(message);
    std::cout << "Client: Sending message: [" << message << "]" << std::endl;

    // Receive acknowledgment from the server
    std::string response = clientChannel.receive();
    std::cout << "Client: Received acknowledgment: [" << response << "]" << std::endl;

    // Stop the client and clean up
    clientChannel.stop();
    delete clientSocket;

    return 0;
}