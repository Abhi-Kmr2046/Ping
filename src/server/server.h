#include <string> 
#include <netinet/in.h>

#include "../interface/clientserver.h"
#include "../interface/consts.h"


class Server : public ClientServer
{
private:

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



};



