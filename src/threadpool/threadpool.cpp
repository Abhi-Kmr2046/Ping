#include <functional>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <unistd.h>
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
        std::vector<std::thread::id> finished_td;
        for(auto rq: finished){
            if(rq.second == true){
                finished_td.push_back(rq.first);
            }
        }

        for(auto fid: finished_td){
            requests[fid].join();
            requests.erase(fid);
            finished.erase(fid);
            std::cout<<"....Thread freed: " << fid <<std::endl;
        }
        mx_pool.unlock();
        sleep(3);
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
        std::cout<<"New Request create with id: " << tid <<std::endl;

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

    child_thread.detach();
    return 0;
}

const int ThreadPool::max_threads = 20;