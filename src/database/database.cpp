#include "database.h"

Database::Database()
{
}

Database::~Database()
{
}

Database* Database::getInstance() 
{
    if(instance == nullptr) {
        instance = new Database();
    }
    return instance;
}

Database* Database::instance = nullptr;