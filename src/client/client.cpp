
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#include "client.h"


Client::Client() 
{
    buffer = new char[BUF];

    // Creating socket file descriptor
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        throw "Failed Socket Creation";
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(SPORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, IP, &addr.sin_addr)
        <= 0) {
        perror("Invalid address/ Address not supported \n");
    }
}

Client::~Client()
{
    
}

Client* Client::getInstance() 
{
    if(instance == nullptr) {
        instance = new Client();
    }
    std::cout<<"Instance pid: "<<getpid()<<std::endl;
    return instance;
    
}

int Client::test()
{
    int status;
    if ((status
         = connect(fd, (struct sockaddr*)&addr,
                   sizeof(addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    if (getsockname(fd, (struct sockaddr*)&local, &len) < 0) {
        perror("getsockname");
    } else {
        printf("Client is using local port %d\n", ntohs(local.sin_port));
    }

    char* hello = "Hello from client";
    send(fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    size_t valread = read(fd, buffer,
                   1024 - 1); 
    printf("%s\n", buffer);

    // closing the connected socket
    close(fd);
    return 0;
}

int Client::connectServer()
{
    int status;
    if ((status
         = connect(fd, (struct sockaddr*)&addr,
                   sizeof(addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    if (getsockname(fd, (struct sockaddr*)&local, &len) < 0) {
        perror("getsockname");
        throw "Connection Failed";
    } else {
        printf("Client is using local port %d\n", ntohs(local.sin_port));
    }
    return 0;
}

int Client::sendMessage(char* message, int len)
{
    sendSocket(fd, message, len);
    return 0;
}

int Client::receiveMessage(char* message)
{
    receiveSocket(fd, message);
    return 0;
}

Client* Client::instance = nullptr;
char* Client::IP = "192.168.0.115";