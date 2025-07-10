
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

#include "server.h"


Server::Server() 
{
    
    buffer = new char[BUF];
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        throw "Failed Socket Creation";
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP);
    address.sin_port = htons(PORT);
}

Server::~Server()
{
    delete buffer;
    close(server_fd);
}

Server* Server::getInstance() 
{
    if(instance == nullptr) {
        instance = new Server();
    }
    std::cout<<"Instance pid: "<<getpid()<<std::endl;
    return instance;
    
}

int Server::bindSocket()
{
    // Forcefully attaching socket to the port
    if (bind(server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        throw "Bind Failed";
    }
    return 1;
}

int Server::test()
{
    char* hello = "Hello from server";
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
         = accept(server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
  
    // subtract 1 for the null
    // terminator at the end
    size_t valread = read(new_socket, buffer,
                   1024 - 1); 
    printf("%s\n", buffer);
    //send(new_socket, hello, strlen(hello), 0);
    //printf("Hello message Received\n");

    close(new_socket);
    return 1;
}

int Server::processRequest()
{
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket
        = accept(server_fd, (struct sockaddr*)&address,
        &addrlen))
        < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

    
    char* message = buffer;

    int status = receiveMessage(message);
    printf("%s\n", message);

    char* messageS = "Hello There Message from Server";
    sendMessage(messageS, strlen(messageS));

    close(new_socket);
    return 1;
}

int Server::sendMessage(char* message, int len)
{
    send(new_socket, message, len, 0);
    return 0;
}

int Server::receiveMessage(char* message)
{
    // subtract 1 for the null
    // terminator at the end
    size_t valread = read(new_socket, message, BUF - 1); 
    message[valread] = 0;
    return 0;
}

int Server::startServer()
{
    int ret = fork();

    if(ret == -1) {
        throw "Process Error";
    } else if(ret == 0) {
        while (true)
        {
            processRequest();
            sleep(1);
        }
    } else {
        std::cout<<"Server Started pid: "<<ret<<std::endl;
        int status;
        wait(&status);
    }
    return 1;
}


Server* Server::instance = nullptr;
char* Server::IP = "192.168.0.115";