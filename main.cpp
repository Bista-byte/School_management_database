#include <iostream>
#include "SQL/sqlite3.h"
using namespace std;

class DataBase
{
private:
    sqlite3 *db = nullptr;

public:
    bool OpenDB(string &filename)
    {
        int rc = sqlite3_open(filename.c_str(), &db);

        if (rc != SQLITE_OK)
        {
            cout << "Couldn't Open Database" << sqlite3_errmsg(db) << endl;
            return false;
        }
        return true;
    }

    bool ExecuteSQL(const string &command)
    {
        if (db == nullptr)
        {
            cout << "No Database opened";
        }

        char *errmsg = nullptr;

        int rc = sqlite3_exec(db, command.c_str(), nullptr, nullptr, &errmsg);

        if (rc != SQLITE_OK)
        {
            cout << "Command execution Failed" << errmsg << endl;
            sqlite3_free(errmsg);
            return false;
        }
        return true;
    }
};

int main(void)
{
}