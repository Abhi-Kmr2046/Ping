#include <string> 
#include <netinet/in.h>

#define PORT 8080
#define BUF  65536


class Server
{
private:
    int server_fd;
    int new_socket;
    char* buffer;
    int opt = 1;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    static Server* instance;

    static char* IP;
private:
    ~Server();
    Server();

public:
    static Server* getInstance();
    int bindSocket();
    int test();
    int startServer();
    int processRequest();
    int sendMessage(char* message, int len);
    int receiveMessage(char* message);


};



