#include <thread>
#include <map>
#include <mutex> 
#include <functional>

class ThreadPool
{
private:
    static const int max_threads;
    std::mutex mx_pool;
    std::thread child_thread;

    std::map<std::thread::id,std::thread> requests;
    std::map<std::thread::id,bool> finished;

    int destroyFinishedThreads();


public:
    ThreadPool();
    ~ThreadPool();

    
    int addRequest(std::function<int(int)>, int);

    int markFinished(std::thread::id);

    int startMonitoring();
};

