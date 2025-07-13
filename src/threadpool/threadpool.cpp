#include <functional>
#include "threadpool.h"

//TODO: Signalling to terminate all the threads


ThreadPool::ThreadPool()
{
    
}

ThreadPool::~ThreadPool()
{
}

int ThreadPool::destroyFinishedThreads()
{
    while (true)
    {
        mx_pool.lock();
        for(auto rq: finished){
            if(rq.second == true){
                requests[rq.first].join();
                requests.erase(rq.first);
                finished.erase(rq.first);
            }
        }
        mx_pool.unlock();
    }
    return 0;
}

int ThreadPool::addRequest(std::function<int(int)> fn, int client_socket)
{
    mx_pool.lock();
        std::thread td(fn, client_socket);
        std::thread::id tid = td.get_id();
        requests.emplace(tid, std::move(td));
        finished.emplace(tid, false);
    mx_pool.unlock();
    return 0;
}
int ThreadPool::markFinished(std::thread::id id)
{
    mx_pool.lock();
        finished[id] = true;
    mx_pool.unlock();
    return 0;
}

int ThreadPool::startMonitoring()
{
    child_thread = std::thread(
        [this] {return this->destroyFinishedThreads();}
    );

    child_thread.join();
    return 0;
}

const int ThreadPool::max_threads = 20;