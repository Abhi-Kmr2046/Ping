#include "client.h"

#include <iostream>
#include <string>
#include <string.h>


int main(int argc, char const* argv[])
{
    Client* cli = Client::getInstance();
    //cli->test();
    char* message = "Hello There Message from Client";
    cli->connectServer();
    cli->sendMessage(message, strlen(message));
    
    char* rec = new char[1024];
    cli->receiveMessage(rec);
    std::cout<<rec<<std::endl;

}