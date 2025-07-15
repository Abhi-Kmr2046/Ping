#include <string> 
#include <netinet/in.h>
#include <mutex> 
#include <thread>
#include <set>

#include "../interface/clientserver.h"
#include "../interface/consts.h"
#include "../threadpool/threadpool.h"
#include "../database/database.h"

class Server : public ClientServer
{
private:

    static Server* instance;
    static char* IP;
    
    struct operation
    {
        int ops = 0;
        std::string filepath;
    };
    
    static char* downloaddir;

    Database* dbinstance;
private:
    ~Server();
    Server();


    int test();
    int retrieveClientRequest();
    
    std::mutex mx_server;
    ThreadPool thread_pool;

    operation parseRequest(std::string);
public:
    static Server* getInstance();
    int bindSocket();
    int startServer();
    int processClientRequest(int );
    int ops0TestMessageServer(int, operation , char*);
    int ops1SendFileServer(int, operation , char*);
    int ops2ReceiveFileServer(int, operation , char*);
};



