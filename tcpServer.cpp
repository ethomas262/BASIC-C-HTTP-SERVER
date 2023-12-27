#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int main() {
    cout << "Attempting to create a server\n";

    // Declare variables for sockets and networking
    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server;
    int server_len;
    int buffer_size = 30720;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error: Failed to initialize server\n";
        return 1;
    }

    // Create a socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET) {
        cerr << "Error: Failed to create socket\n";
        WSACleanup();
        return 1;
    }

    // Bind socket to address
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server.sin_port = htons(8080);
    server_len = sizeof(server);
    if (bind(wsocket, (SOCKADDR*)&server, server_len) != 0) {
        cerr << "Error: Failed to bind socket\n";
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    // Start listening for incoming connections
    if (listen(wsocket, SOMAXCONN) != 0) {
        cerr << "Error: Could not start listening\n";
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    cout << "Listening on 127.0.0.1:8080\n";

    int bytes = 0;
    while (true) {
        // Accept client request
        new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
        if (new_wsocket == INVALID_SOCKET) {
            cerr << "Could not accept\n";
            continue;
        }

        // Read request from client
        char buff[30720] = { 0 };
        bytes = recv(new_wsocket, buff, buffer_size, 0);
        if (bytes < 0) {
            cerr << "Could not read client request\n";
        }

        // Read predefined HTTP response from file
        ifstream in_file("response.http");
        stringstream buffer;
        buffer << in_file.rdbuf();
        in_file.close();
        string serverMessage = buffer.str();

        // Send the HTTP response to the client
        int bytesSent = send(new_wsocket, serverMessage.c_str(), serverMessage.size(), 0);
        if (bytesSent < 0) {
            cerr << "Could not send response\n";
        }

        cout << "Sent response to client\n";
        
        // Close the client socket
        closesocket(new_wsocket);
    }

    // Close the server socket and clean up Winsock
    closesocket(wsocket);
    WSACleanup();

    return 0;
}
