#ifndef SOCKET_H
#define SOCKET_H
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <tchar.h>
class Socket{
    SOCKET serverSocket, acceptSocket;
    int port;

    WSADATA wsaData;
    int wsaerr;
    public:
    Socket(int port = -1);
    int Bind(std::string ip);
    int Listen(void);
    int Accept(void);
    std::string Receive(void);
    void Send(std::string buffer);
    void Initialize(void);
    void setPort(int port);
    void Close(void);
};
#endif