
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
#include <sstream>
#include <stdlib.h> 

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
    return 0;
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

int Server::ops0TestMessageServer(int client_socket, operation op, char* message)
{
    op;
    int status = receiveSocket(client_socket,message);
    // printf("%s\n", message);
    std::cout<<message<<std::endl;

    char* messageS = "Operation 0 Test Connection : Server";
    sendSocket(client_socket, messageS, strlen(messageS));

    sleep(1);
    return 0;
}

int Server::ops1SendFileServer(int client_socket, operation op, char* message)
{
    size_t file_size = 0;
    std::cout << "Operation : " << op.ops << " ";
    std::cout<<"File path: "<<op.filepath<<std::endl;
    if (FILE *fp = fopen(op.filepath.c_str(), "rb")) {
        size_t len = 0;
        while((len = fread(message, 1, sizeof(message), fp)) > 0) {
            file_size += len;
            if(len < sizeof(message)) break;
        }
        fclose(fp);
    }else {
        std::cout<<"File Read Error"<<std::endl;
    }

    // Send File Size
    char file_size_s [10];
    strcpy(file_size_s, std::to_string(file_size).c_str());
    sendSocket(client_socket, file_size_s, strlen(file_size_s));

    if (FILE *fp = fopen(op.filepath.c_str(), "rb")) {
        size_t len = 0;
        while((len = fread(message, 1, sizeof(message), fp)) > 0) {
            //std::cout << len<< std::endl;
            sendSocket(client_socket, message, len);
        }
    }else {
        std::cout<<"File Read Error"<<std::endl;
    }

    // send file
    sleep(1);
    return 0;
}

int Server::ops2ReceiveFileServer(int client_socket, operation op, char* message)
{
    std::cout << "Operation : " << op.ops << " ";
    std::cout << sizeof(message) << std::endl;

    char* filename = new char[1024];
    instance->receiveSocket(client_socket, filename, 1024);
    std::cout<<"Filename : " << filename<<std::endl;

    char* dest_path = new char[1024];
    strcpy(dest_path, downloaddir);
    strcpy(&dest_path[strlen(dest_path)], filename);

    char * file_size_s = new char [1024];
    instance->receiveSocket(client_socket, file_size_s, sizeof(file_size_s));
    size_t file_size = atoi(file_size_s);
    std::cout<<"File Size:" << file_size<<std::endl;

    if (FILE *fp = fopen(dest_path, "wb")) {
        size_t len = 0;
        while(file_size >0) {
            len = instance->receiveSocket(client_socket, message, sizeof(message));
            //std::cout<<file_size << " " << len<<std::endl;
            
            if(file_size < sizeof(message)) len = file_size;

            file_size -= len;
            int fst = 0;
            if ((fst = fwrite(message, 1, len, fp)) < 0 ){
                std::cout<<"file Write error" << std::endl;
            }

        }

        std::cout << "File Transfer Complete" <<std::endl;
    }else {
        std::cout<<"File Read Error"<<std::endl;
    }
    
    
    delete filename;
    delete dest_path;
    delete file_size_s;
    return 0;
}

int Server::processClientRequest(int client_socket)
{
    char* message = new char[BUF];

    int status = receiveSocket(client_socket,message);
    // printf("%s\n", message);
    std::cout<<message<<std::endl;

    char* messageS = "Connection Established - Server";
    sendSocket(client_socket, messageS, strlen(messageS));

    
    status = receiveSocket(client_socket,message);
    
    std::string req(message);
    operation op = parseRequest(req);
    
    switch (op.ops)
    {
        case 0:
        ops0TestMessageServer(client_socket, op, message);
        break;
        case 1:
        ops1SendFileServer(client_socket, op, message);
        break;
        case 2:
        ops2ReceiveFileServer(client_socket, op, message);
        break;
        default:
        break;
    }
    sleep(4);


    close(client_socket);

    std::thread::id tid = std::this_thread::get_id();
    thread_pool.markFinished(tid);
    delete message;

    std::cout<<"Request Completed: " << tid<<std::endl;
    
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

    std::cout<<"Connection Accepted for client: " << client_socket <<std::endl;
    //processClientRequest(client_socket);
    //return 0;
    int st = thread_pool.addRequest(
        [this] (int sock) {return this->processClientRequest(sock);}, 
        client_socket
    );
    
    
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
            thread_pool.startMonitoring();
            retrieveClientRequest();
            sleep(1);
        }
    } else {
        std::cout<<"Server Started pid: "<<ret<<std::endl;
        int status;
        wait(&status);
    }
    return 0;
}

Server::operation Server::parseRequest(std::string req)
{
    operation op;

    std::stringstream req_stream(req);
    req_stream>>op.ops;
    switch (op.ops)
    {
    case 0:
        break;
    case 1:
        req_stream>>op.filepath;
    case 2:
        break;
    default:
        break;
    }
    return op;
}


Server* Server::instance = nullptr;
char* Server::IP = "192.168.0.115";

char* Server::downloaddir = "/home/abhi/Downloads/Server/Download/";
