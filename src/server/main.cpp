#include "server.h"

#include <iostream>
#include <string>
#include <string.h>

int main(int argc, char const* argv[])
{
    Server* ser = Server::getInstance();
    ser->bindSocket();
    ser->startServer();
}