#include "clientserver.h"
#include <unistd.h>
#include <sys/socket.h>

ClientServer::ClientServer()
{
}

ClientServer::~ClientServer()
{
    delete buffer;
    close(fd);
}

int ClientServer::sendSocket(int socket, char* message, int len)
{
    send(socket, message, len, 0);
    return 0;
}

int ClientServer::receiveSocket(int socket, char* message, int len)
{
    // subtract 1 for the null
    // terminator at the end
    size_t valread = read(socket, message, len); 
    //message[valread] = 0;
    return valread;
    
}