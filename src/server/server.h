#include <string> 
#include <netinet/in.h>
#include <mutex> 
#include <thread>
#include <set>

#include "../interface/clientserver.h"
#include "../interface/consts.h"
#include "../threadpool/threadpool.h"

class Server : public ClientServer
{
private:

    static Server* instance;
    static char* IP;
    

private:
    ~Server();
    Server();


    int test();
    int retrieveClientRequest();
    
    std::mutex mx_server;
    ThreadPool thread_pool;

public:
    static Server* getInstance();
    int bindSocket();
    int startServer();
    int processClientRequest(int client_socket);

};



