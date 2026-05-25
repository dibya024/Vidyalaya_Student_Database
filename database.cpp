#include <iostream>
#include "database.h"

using namespace std;

Database :: Database() {

    cout << "Database constructor started\n";
    
    int exit = sqlite3_open("student.db", &db);

    if (exit) {
        cout << "Database connection failed!\n";
    }
    else{
        cout << "Connected to SQLite database!\n";
    }

    createTable();
    loadCache();

}


Database :: ~Database() {
    sqlite3_close(db);
    cout << "Database connection closed.\n";
}


void Database :: createTable(){

    string sql= "CREATE TABLE IF NOT EXISTS students ("
                 "roll_no TEXT PRIMARY KEY,"
                 "name TEXT NOT NULL,"
                 "branch TEXT NOT NULL,"
                 "rank INTEGER,"
                 "age INTEGER,"
                 "sgpa REAL,"
                 "cgpa REAL NOT NULL"
                 ");";

    char* errMsg;

    int exit = sqlite3_exec(db, sql.c_str(), NULL, 0, &errMsg);

    if (exit != SQLITE_OK) {

        cout << "Table creation failed!\n";
        sqlite3_free(errMsg);

    }
    else{
        cout << "Table created sucessfully!\n";
    }

    // string rollIndex= "CREATE INDEX IF NOT EXISTS idx_roll "
    //                   "ON students(roll_no);";
    
    // sqlite3_exec(db, rollIndex.c_str(), NULL, NULL, &errMsg);

    string nameIndex= "CREATE INDEX IF NOT EXISTS idx_name "
                      "ON students(name);";

    sqlite3_exec(db, nameIndex.c_str(), NULL, NULL, &errMsg);

    cout << "Indexes created successfully!\n";

}



bool Database :: addStudent(const Student& s) {

    string sql = "INSERT INTO students "
                 "(roll_no, name, branch, rank, age, sgpa, cgpa) VALUES (?, ?, ?, ?, ?, ?, ?);";

                //  s.getRoll() + "', '" +
                //  s.getName() + "', '" +
                //  s.getBranch() + "', " +

                //  to_string(s.getRank()) + ", " +
                //  to_string(s.getAge()) + ", " +
                //  to_string(s.getSgpa()) + ", " +
                //  to_string(s.getCgpa()) +
                //  ");";

    sqlite3_stmt* stmt;

    int prepare = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (prepare != SQLITE_OK) {

        cout << "SQL prepare failed!\n";
        return false;
    }

    sqlite3_bind_text(stmt, 1, s.getRoll().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, s.getName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, s.getBranch().c_str(), -1, SQLITE_STATIC);

    sqlite3_bind_int(stmt, 4, s.getRank());
    sqlite3_bind_int(stmt, 5, s.getAge());

    sqlite3_bind_double(stmt, 6, s.getSgpa());
    sqlite3_bind_double(stmt, 7, s.getCgpa());

    int step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (step != SQLITE_DONE) {

        cout << "Insertion failed!\n";
        return false;
    }

    studentCache[s.getRoll()] = s;

    cout << "Student added successfully!\n";
    return true;

}




void Database :: viewStudents() {

    string sql = "SELECT * FROM students;";

    auto callback = [](void*, int argc, char** argv, char** colName) {
        cout << "\n-------------------------------------\n";

        for (int i= 0; i < argc; i++) {
            cout << colName[i] << ": "
                 << (argv[i] ? argv[i] : "NULL") << endl;

        }

        return 0;
    };

    char *errMsg;

    int exit = sqlite3_exec(db, sql.c_str(), callback, NULL, &errMsg);

    if (exit != SQLITE_OK) {
        cout << "Failed to get data\n";
        sqlite3_free(errMsg);
    }
}




void Database::searchByName(string name) {

    string sql = "SELECT * FROM students WHERE name = ?;";

    sqlite3_stmt* stmt;
    int prepare =
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if(prepare != SQLITE_OK) {

        cout << "SQL Prepare Failed!\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    bool found = false;

    while(sqlite3_step(stmt) == SQLITE_ROW) {

        found = true;
        cout << "\nStudent Found\n";

        cout << "Roll No: " << sqlite3_column_text(stmt, 0) << endl;
        cout << "Name: " << sqlite3_column_text(stmt, 1) << endl;
        cout << "Branch: " << sqlite3_column_text(stmt, 2) << endl;

        cout << "Rank: " << sqlite3_column_int(stmt, 3) << endl;
        cout << "Age: " << sqlite3_column_int(stmt, 4) << endl;

        cout << "SGPA: " << sqlite3_column_double(stmt, 5) << endl;
        cout << "CGPA: " << sqlite3_column_double(stmt, 6) << endl;
    }

    if(!found) {

        cout << "Student not found!\n";
    }

    sqlite3_finalize(stmt);
}




void Database :: searchByRoll(string roll) {

    if (studentCache.count(roll)) {

        cout << "\n(Cache) Student Found\n";
        studentCache[roll].display();
        return;
    }

    string sql= "SELECT * FROM students WHERE roll_no = ?;";

     sqlite3_stmt* stmt;

    int prepare = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if(prepare != SQLITE_OK) {

        cout << "SQL Prepare Failed!\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, roll.c_str(), -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);

    if(step == SQLITE_ROW) {

        cout << "\n(Database) Student Found\n";

        cout << "Roll No: " << sqlite3_column_text(stmt, 0) << endl;
        cout << "Name: " << sqlite3_column_text(stmt, 1) << endl;
        cout << "Branch: " << sqlite3_column_text(stmt, 2) << endl;

        cout << "Rank: " << sqlite3_column_int(stmt, 3) << endl;
        cout << "Age: " << sqlite3_column_int(stmt, 4) << endl;

        cout << "SGPA: " << sqlite3_column_double(stmt, 5) << endl;
        cout << "CGPA: " << sqlite3_column_double(stmt, 6) << endl;
    }

    else {
        cout << "Student not found!\n";
    }

    sqlite3_finalize(stmt);

}



void Database::deleteByName(string name) {

    string sql = "DELETE FROM students WHERE name = ?;";

    sqlite3_stmt* stmt;
    int prepare =sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if(prepare != SQLITE_OK) {

        cout << "SQL Prepare Failed!\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(step != SQLITE_DONE) {

        cout << "Deletion failed!\n";
        return;
    }

    if(sqlite3_changes(db) > 0) {

        loadCache();
        cout << "Student deleted successfully!\n";
    }

    else {
        cout << "Student not found!\n";
    }
}




void Database::deleteByRoll(string roll) {

    string sql = "DELETE FROM students WHERE roll_no = ?;";

    sqlite3_stmt* stmt;
    int prepare = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if(prepare != SQLITE_OK) {

        cout << "SQL Prepare Failed!\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, roll.c_str(), -1, SQLITE_STATIC);
    int step = sqlite3_step(stmt);

    sqlite3_finalize(stmt);

    if(step != SQLITE_DONE) {

        cout << "Deletion failed!\n";
        return;
    }

    if(sqlite3_changes(db) > 0) {

        studentCache.erase(roll);
        cout << "Student deleted successfully!\n";
    }

    else {

        cout << "Student not found!\n";
    }
}




void Database::selectStudent(string roll) {

    string sql = "SELECT * FROM students WHERE roll_no = ?;";

    sqlite3_stmt* stmt;
    int prepare = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if(prepare != SQLITE_OK) {

        cout << "SQL Prepare Failed!\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, roll.c_str(), -1, SQLITE_STATIC);
    int step = sqlite3_step(stmt);

    if(step == SQLITE_ROW) {

        cout << "\nStudent Record Found!\n";

        cout << "Roll No: " << sqlite3_column_text(stmt, 0) << endl;
        cout << "Name: " << sqlite3_column_text(stmt, 1) << endl;
        cout << "Branch: " << sqlite3_column_text(stmt, 2) << endl;

        cout << "Rank: " << sqlite3_column_int(stmt, 3) << endl;
        cout << "Age: " << sqlite3_column_int(stmt, 4) << endl;

        cout << "SGPA: " << sqlite3_column_double(stmt, 5) << endl;
        cout << "CGPA: " << sqlite3_column_double(stmt, 6) << endl;
    }

    else {
        cout << "Student not found!\n";
    }

    sqlite3_finalize(stmt);
}




void Database::updateStudent(string roll, string column, string value) {

    string sql = "UPDATE students SET " + column + " = ? WHERE roll_no = ?;";

    sqlite3_stmt* stmt;
    int prepare =
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if(prepare != SQLITE_OK) {

        cout << "SQL Prepare Failed!\n";
        return;
    }

    sqlite3_bind_text(stmt, 1, value.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, roll.c_str(), -1, SQLITE_STATIC);

    int step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if(step != SQLITE_DONE) {

        cout << "Update failed!\n";
        return;
    }

    if(sqlite3_changes(db) > 0) {

        loadCache();
        cout << "Student updated successfully!\n";
    }

    else {
        cout << "Student not found!\n";
    }
}




void Database :: loadCache() {

    studentCache.clear();

    string sql = "SELECT * FROM students;";

    sqlite3_stmt* stmt;
    int prepare = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (prepare != SQLITE_OK ){ 
        cout << "Cache loading failed!\n";
        return;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {

        string roll = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string branch = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

        int rank = sqlite3_column_int(stmt, 3);
        int age = sqlite3_column_int(stmt, 4);

        float sgpa = sqlite3_column_double(stmt, 5);
        float cgpa = sqlite3_column_double(stmt, 6);

        Student s(name, roll, branch, rank, age, sgpa, cgpa);
        studentCache[roll] = s;


    }

    sqlite3_finalize(stmt);
    cout << "Student cache loaded successfully!\n";

}