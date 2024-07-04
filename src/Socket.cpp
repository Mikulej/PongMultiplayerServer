#include "Socket.h"

// WSADATA Socket::wsaData;
// int Socket::wsaerr;
void Socket::Initialize(){
    WORD wVersionRequested = MAKEWORD(2,2);
    wsaerr = WSAStartup(wVersionRequested, &wsaData);
    if(wsaerr != 0){
        std::cout << "The Winsock dll not found!" << std::endl;
    }
    else{
        std::cout << "The Winsock dll found!" << std::endl;
        std::cout << "Status: " << wsaData.szSystemStatus << std::endl;
    }

}
Socket::Socket(int port):port(port){
    Initialize();
    serverSocket = INVALID_SOCKET;
    serverSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if(serverSocket==INVALID_SOCKET){
        std::cout << "Error at socket()" << WSAGetLastError() << std::endl;
        WSACleanup();
    }
    else{
        std::cout << "socket() is OK!" << std::endl;
    }
}
int Socket::Bind(std::string ip){
    if(port==-1){
        std::cout << "bind() failed! - port is -1" << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    sockaddr_in service;
    service.sin_family = AF_INET;
    //InetPton(AF_INET,_T("127.0.0.1"), &service.sin_addr.s_addr);
    std::wstring stemp = std::wstring(ip.begin(), ip.end());
    LPCSTR sw = ip.c_str();
    InetPton(AF_INET,sw, &service.sin_addr.s_addr);
    service.sin_port = htons(port);
    if(bind(serverSocket,(SOCKADDR*)&service,sizeof(service))==SOCKET_ERROR){
        std::cout << "bind() failed!" << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 0;
    }
    else{
        std::cout << "bind() is OK!" << std::endl;
    }
}
int Socket::Listen(){
    if(listen(serverSocket,1)==SOCKET_ERROR){
        std::cout<<"listen(): Error listening on socket" << std::endl;
    }
    else{
        std::cout<<"listen() is OK, waiting for connections..." << std::endl;
    }
}
int Socket::Accept(){
    acceptSocket = accept(serverSocket,NULL,NULL);
    if(acceptSocket==INVALID_SOCKET){
        std::cout<<"Accept failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 0;
    }
    std::cout<<"Accepted connection" << std::endl;
}
std::string Socket::Receive(){
    char buffer[100] ={0};
    int byteCount = recv(acceptSocket,buffer,sizeof(buffer),0);
    if(byteCount > 0){
        //std::cout<<"Received message: "<< buffer << std::endl;
    }
    else{
       WSACleanup();
    }
    return buffer;
}

void Socket::Send(std::string buffer){
    int byteCount = send(acceptSocket,buffer.data(),buffer.size(),0);
    if(byteCount > 0){
        //std::cout<<"Sent message: "<< buffer << std::endl;
    }
    else{
       WSACleanup();
    }
}
void Socket::setPort(int port){
    this->port = port;
}
void Socket::Close(void){
    closesocket(serverSocket);
    //WSACleanup();
}