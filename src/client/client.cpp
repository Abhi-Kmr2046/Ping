
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
    buffer = new char[BUFS];

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
        std::cout<<"Connection Failed"<<std::endl;
        return -1;
    }

    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    if (getsockname(fd, (struct sockaddr*)&local, &len) < 0) {
        perror("getsockname");
    } else {
        std::cout<<"Client is using local port " << ntohs(local.sin_port)<<std::endl;
    }

    char* hello = "Hello from client";
    send(fd, hello, strlen(hello), 0);
    std::cout<<"Hello message sent\n"<<std::endl;
    size_t valread = read(fd, buffer,
                   BUFS - 1); 
    // printf("%s\n", buffer);
    std::cout<<buffer<<std::endl;

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
        std::cout<<"Connection Failed"<<std::endl;
        return -1;
    }
    struct sockaddr_in local;
    socklen_t len = sizeof(local);
    if (getsockname(fd, (struct sockaddr*)&local, &len) < 0) {
        perror("getsockname");
        throw "Connection Failed";
    } else {
        std::cout<<"Client is using local port " << ntohs(local.sin_port)<<std::endl;
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
    size_t len = receiveSocket(fd, message);
    return len;
}
int Client::ops0TestMessageClient()
{   
    char* req_string = "0";
    instance->sendMessage(req_string, strlen(req_string));

    char* message = "Operation 0 Test Connection : Server";
    instance->sendMessage(message, strlen(message));
    
    char* rec = new char[BUFS];
    //sleep(10);
    instance->receiveMessage(rec);
    std::cout<<rec<<std::endl;
    delete rec;
    return 0;
}
int Client::ops1ReceiveFileClient(char* destfilepath, char* serverpath)
{   
    char* req_string = new char[256];
    req_string[0] = '1';
    req_string[1] = ' ';
    strcpy(&req_string[2], serverpath);
    instance->sendMessage(req_string, strlen(req_string));

    char * rec = new char [BUFS];
    instance->receiveMessage(rec);
    size_t file_size = atoi(rec);
    std::cout<<"File Size:" << file_size<<std::endl;

    char * message = new char[BUF];


    if (FILE *fp = fopen(destfilepath, "wb")) {
        size_t len = 0;
        while(file_size >0) {
            len = receiveMessage(message);
            //std::cout<<file_size << " " << len<<std::endl;
            
            if(file_size < BUF) len = file_size;

            file_size -= len;
            int fst = 0;
            if ( (fst = fwrite(message, 1, len, fp)) < 0 ){
                std::cout<<"file Write error" << std::endl;
            }

        }

        std::cout << "File Transfer Complete" <<std::endl;
    }else {
        std::cout<<"File Read Error"<<std::endl;
    }
    
    
    delete req_string;
    delete rec;
    delete message;
    return 0;
}

int Client::ops2SendFileClient(char* filename, char* filepath)
{
    char* req_string = new char[256];
    req_string[0] = '2';
    req_string[1] = 0;
    instance->sendMessage(req_string, strlen(req_string));


    size_t file_size = 0;
    char* message = new char[BUF];
    if (FILE *fp = fopen(filepath, "rb")) {
        size_t len = 0;
        while((len = fread(message, 1, BUF, fp)) > 0) {
            file_size += len;
            if(len < BUF) break;
        }
        fclose(fp);
    }else {
        std::cout<<"File Read Error"<<std::endl;
    }

    // Send File Name
    // char file_size_s [1024];
    // strcpy(file_size_s, std::to_string(file_size).c_str());
    sendMessage(filename, BUFS);
    //std::cout<<filename << " " << sizeof(filename)<<std::endl;
    
    // Send File Size
    char file_size_s [BUFS];
    strcpy(file_size_s, std::to_string(file_size).c_str());
    sendMessage(file_size_s, BUFS);



    if (FILE *fp = fopen(filepath, "rb")) {
        size_t len = 0;
        while((len = fread(message, 1, BUF, fp)) > 0) {
            sendMessage(message, BUF);
        }
    }else {
        std::cout<<"File Read Error"<<std::endl;
    }

    // send file
    delete message;
    sleep(1);
    return 0;
}

Client* Client::instance = nullptr;
char* Client::IP = "192.168.0.115";