cmake_minimum_required(VERSION 3.9.1)
project(Server)
set(CMAKE_CXX_STANDARD 14)
find_package(Boost REQUIRED COMPONENTS headers)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)


add_executable(server 
                main.cpp 
                server.cpp 
                server.h .
                ../interface/clientserver.cpp 
                ../interface/clientserver.h
                ../threadpool/threadpool.cpp 
                ../threadpool/threadpool.h
                ../database/database.cpp
                ../database/database.h)

target_link_libraries(server PRIVATE Boost::headers Threads::Threads OpenSSL::Crypto OpenSSL::SSL)
