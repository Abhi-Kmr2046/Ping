#include <string> 
#include <netinet/in.h>

#include "../interface/clientserver.h"
#include "../interface/consts.h"



class Client : public ClientServer
{
private:

    static Client* instance;

    static char* IP;
private:
    ~Client();
    Client();

    public:
    static Client* getInstance();
    int test();
    int connectServer();

public:
    int sendMessage(char* message, int len);
    int receiveMessage(char* message);

};
