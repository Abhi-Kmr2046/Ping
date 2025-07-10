
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
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        throw "Failed Socket Creation";
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SPORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, IP, &serv_addr.sin_addr)
        <= 0) {
        perror("Invalid address/ Address not supported \n");
    }
}

Client::~Client()
{
    delete buffer;
    close(client_fd);
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
         = connect(client_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    if (getsockname(client_fd, (struct sockaddr*)&local, &len) < 0) {
        perror("getsockname");
    } else {
        printf("Client is using local port %d\n", ntohs(local.sin_port));
    }

    char* hello = "Hello from client";
    send(client_fd, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    size_t valread = read(client_fd, buffer,
                   1024 - 1); 
    printf("%s\n", buffer);

    // closing the connected socket
    close(client_fd);
    return 0;
}

int Client::connectServer()
{
    int status;
    if ((status
         = connect(client_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    return 0;
    if (getsockname(client_fd, (struct sockaddr*)&local, &len) < 0) {
        perror("getsockname");
        throw "Connection Failed";
    } else {
        std::cout<<"there"<<std::endl;
        printf("Client is using local port %d\n", ntohs(local.sin_port));
    }
    return 0;
}

int Client::sendMessage(char* message, size_t len)
{
    send(client_fd, message, len, 0);
    return 0;
}

int Client::receiveMessage(char* message)
{
    // subtract 1 for the null
    // terminator at the end
    size_t valread = read(client_fd, message, BUF - 1); 
    message[valread] = 0;
    return 0;
}

Client* Client::instance = nullptr;
char* Client::IP = "192.168.0.115";