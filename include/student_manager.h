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

        void viewMyData(std::string roll);
        
        bool addStudent();

        void viewStudents();
        void searchStudent();
        void deleteStudent();
        void updateStudent();
        
};

#endif