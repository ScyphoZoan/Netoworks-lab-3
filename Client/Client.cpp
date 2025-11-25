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
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "ERROR: Failed to start WSA" << endl;
        return EXIT_FAILURE;
    }
#endif

    return 0;
}
