#include "TCPSocket.hpp"
#include "ServerChannel.hpp"


int main()
{
    int port = 8080;  // Port to listen on
    std::string ip = "";  // Empty string for unicast (use multicast IP if needed)

    // Create the socket for the server (TCP socket)
    Socket *serverSocket = new TCPSocket(); // Assuming TCPSocket is a subclass of Socket
    ServerChannel serverChannel(serverSocket, port, ip);  // Create ServerChannel

    std::cout << "Server: Waiting for client connection..." << std::endl;
    serverChannel.start();  // Start the server (bind and listen for connections)
    
    std::string clientIP = serverChannel.getClientIP();
    // Assuming the client is connected
    std::cout << "Server: Client connected from IP: ["<< clientIP <<"]." << std::endl;

    // Receive the message from the client
    std::string message = serverChannel.receive();
    std::cout << "Server: Received message: [" << message << "]" << std::endl;

    // Send acknowledgment with current temperature
    std::string response = "Message received. Current Temperature is 39°C.";
    serverChannel.send(response);
    std::cout << "Server: Sending acknowledgment. Current Temperature is 39°C" << std::endl;

    // Stop the server and clean up
    serverChannel.stop();
    delete serverSocket;

    return 0;
}