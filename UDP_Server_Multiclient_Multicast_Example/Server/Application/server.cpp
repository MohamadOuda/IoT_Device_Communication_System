#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <string>
#include "UDPSocket.hpp"  // Assuming this includes necessary functionality from your classes
#include "ServerChannel.hpp"

const std::string MULTICAST_IP = "239.0.0.1"; // Example multicast address
const int PORT = 8080; // Multicast port

std::string getCurrentTime() {
    // Get the current system time
    auto now = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    // Convert time to string format
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    return std::string(buffer);
}

void startServer() {
    // Create a UDP socket for multicast communication
    UDPSocket udpSocket(CommunicationType::MULTICAST);
    
    // Use ServerChannel to handle communication
    ServerChannel serverChannel(&udpSocket, PORT, MULTICAST_IP);

    // Bind the socket to the multicast address
    serverChannel.start();

    // Periodically send live time to the multicast group
    while (true) {
        std::string currentTime = getCurrentTime();
        std::string message = "Live time: " + currentTime;
        serverChannel.send(message); // Send multicast message to all clients
        std::cout << "Server: Sending multicast message: [" << message << "]" << std::endl;
        
        // Sleep for 5 seconds before sending the next message
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    // Clean up (this will be handled by the destructor of ServerChannel)
    serverChannel.stop();
}

int main() {
    std::cout << "Starting UDP Multicast Server..." << std::endl;
    startServer();
    return 0;
}

