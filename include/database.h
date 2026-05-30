#ifndef DATABASE_H
#define DATABASE_H


#include "student.h"
#include "sqlite/sqlite3.h"

#include <unordered_map>
#include <string>



class Database {

    private:
        sqlite3* db;
        std::unordered_map<std::string, Student> studentCache;

    public:

        Database();
        ~Database();

        
        void loadCache();
        void createTable();

        bool addStudent(const Student& s);
        void viewStudents();

        void searchByName(std::string name);
        void searchByRoll(std::string roll);

        void deleteByRoll(std::string roll);

        void selectStudent(std::string roll_no);

        void updateStudent(std::string roll_no, std::string column, std::string value);

        void showStatistics();

        int getBranchRank(std::string roll);
        int getOverallRank(std::string roll);

};

#endif