#include <string> 





class Database
{
private:
    static Database* instance;
    Database();
    ~Database();
public:
    static Database* getInstance();
};


