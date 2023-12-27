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
    SOCKET wsocket;
    SOCKET new_wsocket;
    WSADATA wsaData;
    struct sockaddr_in server;
    int server_len;
    int buffer_size = 30720;

    // initialize
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Error: Failed to initialize server\n";
        return 1;
    }

    // create a socket
    wsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (wsocket == INVALID_SOCKET) {
        cerr << "Error: Failed to create socket\n";
        WSACleanup();
        return 1;
    }

    // bind socket to address
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

    if (listen(wsocket, SOMAXCONN) != 0) {
        cerr << "Error: Could not start listening\n";
        closesocket(wsocket);
        WSACleanup();
        return 1;
    }

    cout << "Listening on 127.0.0.1:8080\n";

    int bytes = 0;
    while (true) {
        // accept client request
        new_wsocket = accept(wsocket, (SOCKADDR*)&server, &server_len);
        if (new_wsocket == INVALID_SOCKET) {
            cerr << "Could not accept\n";
            continue;
        }

        // read request
        char buff[30720] = { 0 };
        bytes = recv(new_wsocket, buff, buffer_size, 0);
        if (bytes < 0) {
            cerr << "Could not read client request\n";
        }

        ifstream in_file("response.http");
        stringstream buffer;
        buffer << in_file.rdbuf();
        in_file.close();
        string serverMessage = buffer.str();

        int bytesSent = send(new_wsocket, serverMessage.c_str(), serverMessage.size(), 0);
        if (bytesSent < 0) {
            cerr << "Could not send response\n";
        }

        cout << "Sent response to client\n";
        closesocket(new_wsocket);
    }

    closesocket(wsocket);
    WSACleanup();

    return 0;
}