
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
#include <thread>
#include <mutex>


#include "server.h"


Server::Server() 
{
    
    buffer = new char[BUF];
    
    // Creating socket file descriptor
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        throw "Failed Socket Creation";
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(IP);
    addr.sin_port = htons(PORT);
}

Server::~Server()
{
    
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
    if (bind(fd, (struct sockaddr*)&addr,
             sizeof(addr))
        < 0) {
        perror("bind failed");
        throw "Bind Failed";
    }
    return 1;
}

int Server::test()
{
    char* hello = "Hello from server";
    if (listen(fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int new_socket;
    if ((new_socket
         = accept(fd, (struct sockaddr*)&addr,
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

int Server::processClientRequest(int client_socket)
{
    char* message = buffer;

    int status = receiveSocket(client_socket,message);
    printf("%s\n", message);

    char* messageS = "Hello There Message from Server";
    sendSocket(client_socket, messageS, strlen(messageS));

    sleep(10);
    close(client_socket);

    return 0;
}

int Server::retrieveClientRequest()
{
    if (listen(fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int client_socket;
    if ((client_socket
        = accept(fd, (struct sockaddr*)&addr,
        &addrlen))
        < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

    printf("Connection Accepted for client: %d\n", client_socket );
    //processClientRequest(client_socket);
    //return 0;
    int st = thread_pool.addRequest(
        [this] (int sock) {return this->processClientRequest(sock);}, 
        client_socket
    );
    
    
    return 1;
}


int Server::startServer()
{
    int ret = fork();

    if(ret == -1) {
        throw "Process Error";
    } else if(ret == 0) {
        while (true)
        {
            thread_pool.startMonitoring();
            retrieveClientRequest();
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
