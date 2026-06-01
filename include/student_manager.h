#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "student.h"
#include "database.h"
#include "auth_manager.h"



class StudentManager {

    private:
        Database db;
        AuthManager& auth;
    
    public:

        StudentManager(AuthManager& auth);
        
        bool addStudent();

        void viewStudents();
        void searchStudent();
        void deleteStudent();
        void updateStudent();

        void showStatistics();

        void viewMyData(std::string roll);
        void viewMyResult(std::string roll);

        bool changeUsername(std::string roll);
        bool changePassword(std::string roll);

        void viewActivityLogs();

        void exportToCsv();
        void showLeaderboard();

        void addStudentResult();
        void updateStudentResult();
        
};

#endif