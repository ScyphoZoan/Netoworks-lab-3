#include <iostream>
#include <cstring>
#include <string>
#include <vector>

#ifdef _MSVC_LANG

// Windows
// #define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#define close closesocket

#else

// Unix
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define WSACleanup()

#endif

using namespace std;


int main(int argc, char* argv[])
{
    // Start WSA (Windows)
#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "ERROR: Failed to start WSA" << endl;
        return EXIT_FAILURE;
    }
#endif
    // open the server socket
    SOCKET ServerSocket;
    ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ServerSocket == INVALID_SOCKET)
    {
        cout << "ERROR: Failed to create ServerSocket" << endl;
        return EXIT_FAILURE;
    }

    // bind the socket, giving it information it needs for configuration
    sockaddr_in SvrAddr;
    SvrAddr.sin_family = AF_INET;
    SvrAddr.sin_addr.s_addr = INADDR_ANY;
    SvrAddr.sin_port = htons(27000);
    if (bind(ServerSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
    {
        close(ServerSocket);
        WSACleanup();
        cout << "ERROR: Failed to bind ServerSocket" << endl;
        return EXIT_FAILURE;
    }

    //the sercer is now "listening" for an incoming connection to start a three way handshake
    if (listen(ServerSocket, 1) == SOCKET_ERROR)
    {
        close(ServerSocket);
        WSACleanup();
        cout << "ERROR: Failed to configure ServerSocket" << endl;
        return EXIT_FAILURE;
    }

    //Accept; when a SYN request comes into the application, the accept will complete the handshake
    SOCKET ConnectionSocket;
    ConnectionSocket = SOCKET_ERROR;
    if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR)
    {
        close(ServerSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }

    //Send and receive data
    char recvBuff[256] = {};

    size_t bytesRead = recv(ConnectionSocket, recvBuff, sizeof(recvBuff), 0);
    string recvStr(recvBuff, bytesRead);

    cout << recvStr << endl;

    string message("Hello world");
    send(ConnectionSocket, message.c_str(), (int)message.length(), 0);


    //cleanup
    close(ConnectionSocket);
    close(ServerSocket);
    WSACleanup();
    return 0;
}
