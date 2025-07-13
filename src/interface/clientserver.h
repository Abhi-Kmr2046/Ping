#include <netinet/in.h>
#include "consts.h"

class ClientServer
{
public:
    int fd;
    char* buffer;
    int opt = 1;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);

    ClientServer();
    ~ClientServer();

    static ClientServer* getInstance();

    int virtual test() = 0;

    int sendSocket(int socket, char* message, int len);
    int receiveSocket(int socket, char* message, int len = BUF);

};

