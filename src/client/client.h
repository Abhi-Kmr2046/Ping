#include <string> 
#include <netinet/in.h>

#define SPORT 8080
#define BUF  65536

class Client
{
private:
    int client_fd;
    int new_socket;
    char* buffer;
    int opt = 1;
    struct sockaddr_in serv_addr;
    socklen_t addrlen = sizeof(serv_addr);

    static Client* instance;

    static char* IP;
private:
    ~Client();
    Client();

    public:
    static Client* getInstance();
    int test();
    int connectServer();

    int sendMessage(char* message, size_t len);
    int receiveMessage(char* message);
};
