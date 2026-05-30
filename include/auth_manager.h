#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "sqlite3.h"
#include <nlohmann/json.hpp>


using json = nlohmann::json;


class AuthManager {

    private:
        sqlite3* auth_db;
        json users_json;

        std::unordered_map<std::string, std::string> studentPasswordCache;
        std::unordered_map<std::string, std::string> rollCache;
        std::unordered_set<std::string> usedRolls;

    public:
        AuthManager();
        ~AuthManager();

        void connectDatabase();
        void createStudentAuthTable();
        void loadStudentCache();
        void loadJsonUsers();

        std::string login(std::string username, std::string password);
        bool registerStudent(std::string username, std::string password, std::string roll);

        std::string getStudentRoll(std::string username);

        std::string hashPassword(std::string password);

        bool deleteStudentAuthByRoll(std::string roll);

};


#endif