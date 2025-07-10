#include "client.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>


int main(int argc, char const* argv[])
{
    Client* cli = Client::getInstance();
    //cli->test();
    char* message = "Hello There Message from Client";
    cli->connectServer();
    cli->sendMessage(message, strlen(message));
    
    char* rec = new char[1024];
    cli->receiveMessage(rec);
    cout<<rec<<endl;

}