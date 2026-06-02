#include <iostream>
#include "auth_manager.h"


using namespace std;



AuthManager::AuthManager() {

    connectDatabase();
    createStudentAuthTable();
    loadStudentCache();
    loadJsonUsers();
}



AuthManager::~AuthManager() {

    sqlite3_close(auth_db);
}



void AuthManager::connectDatabase() {

    int exit = sqlite3_open("data/auth.db", &auth_db);        //SQLite returns 0(success) and non-zero(failure).

    // if(exit) {
    //     cout << "Auth Database Connection Failed!\n";
    // }
    // else {
    //     cout << "Auth Database Connected\n";
    // }

}




void AuthManager::createStudentAuthTable() {

    string sql ="CREATE TABLE IF NOT EXISTS students_auth ("
                "roll TEXT PRIMARY KEY,"
                "username TEXT UNIQUE,"
                "password TEXT"
                ");";

    char* errMsg = nullptr;

    int exit = sqlite3_exec(auth_db, sql.c_str(), nullptr, nullptr, &errMsg);

    if(exit != SQLITE_OK) {

        cout << "Table Creation Failed : "<< errMsg << endl;
        sqlite3_free(errMsg);
    }
    // else 
    // {
    //     cout << "Student Auth Table Ready\n";
    // }

}




void AuthManager::loadJsonUsers() {

    ifstream file("data/users.json");

    if(!file.is_open()) {

        cout << "users.json not found!\n";
        return;
    }

    file >> users_json;

}




void AuthManager::loadStudentCache() {

    studentPasswordCache.clear();
    rollCache.clear();
    usedRolls.clear();

    string sql = "SELECT username, password, roll FROM students_auth;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(auth_db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        
        cout << "SQL preparation failed!\n";
        return;

    }

    while(sqlite3_step(stmt) == SQLITE_ROW) {

        string username = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string password =reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));

        string roll = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        studentPasswordCache[username] = password;
        rollCache[username] = roll;
        usedRolls.insert(roll);
    }

    sqlite3_finalize(stmt);

}




string AuthManager::login(string username, string password) {

    string hashedPass= hashPassword(password);


    // if(users_json["admins"].contains(username)) {

    //     if(users_json["admins"][username] == hashedPass) {
    //         return "admin";
    //     }
    // }
    if(users_json["admins"].contains(username)) {

        if(users_json["admins"][username] == password) {
            return "admin";
        }
    }


    // if(users_json["teachers"].contains(username)) {

    //     if(users_json["teachers"][username] == hashedPass) {
    //         return "teacher";
    //     }
    // }
    if(users_json["teachers"].contains(username)) {

        if(users_json["teachers"][username] == password) {
            return "teacher";
        }
    }


    if(studentPasswordCache.count(username)) {

        if(studentPasswordCache[username] == hashedPass) {
            return "student";
        }
    }

    return "invalid";

}




bool AuthManager::registerStudent(string username, string password, string roll) {

    if (usedRolls.count(roll)) {

        cout << "Roll number already exists!\n";
        return false;
    }

    if (studentPasswordCache.count(username)) {

        cout << "Username already exists!\n";
        return false;
    }

    string hashedPassword = hashPassword(password);

    string sql = "INSERT INTO students_auth (roll, username, password) VALUES (?, ?, ?);";

    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(auth_db, sql.c_str(), -1, &stmt, nullptr);

    sqlite3_bind_text(stmt, 1, roll.c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, hashedPassword.c_str(), -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    if(step != SQLITE_DONE) {

        cout << "Student Registration Failed!\n";
        return false;
    }


    studentPasswordCache[username] = hashedPassword;
    rollCache[username] = roll;

    usedRolls.insert(roll);

    cout << "Student Registered Successfully\n";
    return true;

}




string AuthManager::getStudentRoll(string username) {

    if(rollCache.count(username)) {
        return rollCache[username];
    }

    return "";
    
}



string AuthManager :: hashPassword (string password) {

    const unsigned long long p = 31;
    const unsigned long long MOD = 1e9 + 7;

    unsigned long long hash = 0;
    unsigned long long power = 1;

    for (unsigned char c : password) {

        unsigned long long val = static_cast<unsigned long long>(c);

        hash = (hash + (val * power) % MOD) % MOD;
        power = (power * p) % MOD;
    }

    return to_string(hash);
}





bool AuthManager :: deleteStudentAuthByRoll(string roll) {

    string sql= "DELETE FROM students_auth WHERE roll = ?;";
    sqlite3_stmt* stmt;

    sqlite3_prepare_v2(auth_db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, roll.c_str(), -1, SQLITE_STATIC);

    int step= sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(step != SQLITE_DONE) {
        return false;
    }

    loadStudentCache();

    return true;
}





bool AuthManager :: verifyCredentials(string roll, string username, string password) {

    if (!studentPasswordCache.count(username)){
        return false;
    }

    if (rollCache[username] != roll) {
        return false;
    }

    string hashedPassword = hashPassword(password);
    if (studentPasswordCache[username] != hashedPassword) {
        return false;
    }
    
    return true;
}




bool AuthManager :: changeUsername(string roll, string newUsername) {

    if (studentPasswordCache.count(newUsername)) {
        cout << "Username not available!\n";
        return false;
    }

    string sql = "UPDATE students_auth SET username = ? WHERE roll = ?;";

    sqlite3_stmt* stmt;

    int prepare = sqlite3_prepare_v2(auth_db, sql.c_str(), -1, &stmt, nullptr);
    if (prepare != SQLITE_OK) {
        cout << "SQL prepare failed!\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, newUsername.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, roll.c_str(), -1, SQLITE_STATIC);

    int step= sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (step != SQLITE_DONE) {
        cout << "Username updatation failed!\n";
        return false;
    }

    loadStudentCache();
    cout << "Username updated successfully";

    return true;
}




bool AuthManager :: changePassword(string roll, string newPassword) {

    string hashedPassword = hashPassword(newPassword);

    string sql = "UPDATE students_auth SET password = ? WHERE roll = ?;";

    sqlite3_stmt* stmt;

    int prepare = sqlite3_prepare_v2(auth_db, sql.c_str(), -1, &stmt, nullptr);
    if (prepare != SQLITE_OK) {
        cout << "SQL prepare failed!\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, hashedPassword.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, roll.c_str(), -1, SQLITE_STATIC);

    int step= sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (step != SQLITE_DONE) {
        cout << "Password updatation failed!\n";
        return false;
    }

    loadStudentCache();
    cout << "Password updated successfully";

    return true;
}