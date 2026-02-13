#include <iostream>
#include "SQL/sqlite3.h"
using namespace std;

bool headerprint = false;

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

    static int callback(void *usdata, int argc, char **argv, char **colname)
    {

        if (!headerprint)
        {
            for (int i = 0; i < argc; i++)
            {
                printf("%-10s", colname[i]);
            }
            cout << endl;
            headerprint = true;
        }

        for (int i = 0; i < argc; i++)
        {
            printf("%-10s", (argv[i] ? argv[i] : "NULL"));
        }
        cout << endl;

        return 0;
    }

    bool QuerySQL(string &command)
    {
        char *errmsg = nullptr;

        int rc = sqlite3_exec(db, command.c_str(), callback, nullptr, &errmsg);

        if (rc != SQLITE_OK)
        {
            cout << "SQL query failed: " << errmsg << endl;
            sqlite3_free(errmsg);
            return false;
        }
        return true;
    }
};

class Person : public DataBase
{
private:
    int age;
    int grade;
    string name;
    string DOB;
    string position;
    string email;

public:
    Person(string &filename)
    {
        OpenDB(filename);

        string createtable =
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "age INTEGER,"
            "grade INTEGER,"
            "name TEXT,"
            "DOB TEXT,"
            "position TEXT,"
            "email TEXT)";

        ExecuteSQL(createtable);
    }

    bool insert_person(int age, int grade, const string &name, const string &DOB, const string &position, const string &email)
    {
        string insert_command = "INSERT INTO users (age,grade,name,DOB,position,email) VALUES (" + to_string(age) + "," + to_string(grade) + ",'" + name + "','" + DOB + "','" + position + "','" + email + "');";
        return ExecuteSQL(insert_command);
    }
};

int main(void)
{
    string filename = "School_file";
    Person person(filename);

    person.insert_person(15, 10, "Bob", "2002/1/1", "teacher", "bistashasha@gmail.com");
    person.insert_person(15, 10, "Sam", "2003/1/1", "student", "samshasha@gmail.com");

    // Query and print all students
    string sqlSelect = "SELECT * FROM users;";
    cout << "Database contents:\n";
    person.QuerySQL(sqlSelect);

    return 0;
}