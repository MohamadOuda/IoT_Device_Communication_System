#include <iostream>
#include <string>
#include "UDPSocket.hpp"  // Assuming this includes necessary functionality from your classes
#include "ClientChannel.hpp"

const std::string MULTICAST_IP = "239.0.0.1"; // Example multicast address
const int PORT = 8080; // Multicast port

void startClient(int clientId) {
    // Create a UDP socket for multicast communication
    UDPSocket udpSocket(CommunicationType::MULTICAST);

    // Use ClientChannel to handle communication
    ClientChannel clientChannel(&udpSocket, PORT, MULTICAST_IP);

    // Start listening for multicast messages
    clientChannel.start();

    // Receive multicast messages (live time) from the server
    while (true) {
        std::string message = clientChannel.receive(); // Receive multicast message
        std::cout << "Client " << clientId << ": Received multicast message: [" << message << "]" << std::endl;
    }

    // Clean up (this will be handled by the destructor of ClientChannel)
    clientChannel.stop();
}

int main() {
    std::cout << "Starting UDP Multicast Client 1..." << std::endl;
    startClient(1);  // Start client 1
    
    return 0;
}

