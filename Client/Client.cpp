#include <iostream>
#include <cstring>
#include <string>

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

    //set up socket
    SOCKET ClientSocket;
    ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ClientSocket == INVALID_SOCKET)
    {
        WSACleanup();
        cout << "ERROR: Failed to create a socket." << endl;
        return EXIT_FAILURE;
    }

    //start connection
    SOCKADDR_IN ServerAddr;
    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(27000);
    ServerAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(ClientSocket, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
    {
        closesocket(ClientSocket);
        WSACleanup();
        cout << "ERROR: Failed to connect to the server." << endl;
        return EXIT_FAILURE;
    }

    //send/receive
    string message("hello world2");
    send(ClientSocket, message.c_str(), (int)message.length(), 0);

    char recvBuff[256] = {};
    size_t bytesRead = recv(ClientSocket, recvBuff, sizeof(recvBuff), 0);
    string recvStr(recvBuff, bytesRead);

    cout << recvStr << endl;

    //cleanup
    close(ClientSocket);
    WSACleanup();
    return 0;
}
