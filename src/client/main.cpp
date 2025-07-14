#include "client.h"

#include <iostream>
#include <string>
#include <string.h>
#include <unistd.h>


int main(int argc, char const* argv[])
{
    Client* cli = Client::getInstance();
    //cli->test();
    char* message = "Connection Established - Client";
    cli->connectServer();
    cli->sendMessage(message, strlen(message));
    
    char* rec = new char[1024];
    //sleep(10);
    cli->receiveMessage(rec);
    std::cout<<rec<<std::endl;

    //cli->ops0TestMessageClient();

    // char* servpath = "/home/abhi/Downloads/Server/back.jpg";
    // char* clipath = "/home/abhi/Downloads/Client/back.jpg";
    // cli->ops1ReceiveFileClient(clipath, servpath);

    char* filepath = "/home/abhi/Downloads/Server/back.jpg";
    char* filename = "back.jpg";
    cli->ops2SendFileClient(filename, filepath);

}