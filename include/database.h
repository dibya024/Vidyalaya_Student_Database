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

        void viewStudentProfile(std::string roll);
        void viewStudentResult(std::string roll);

        void exportToCSV();
        void Leaderboard();

        void createResultTable();
        bool addResult(std::string roll, int semester, std::string subject, int credits, std::string grade);
        bool updateResult(int resultId, std::string newGrade);

        int gradeToPoints(std::string grade);

        double calculateSGPA(std::string roll, int semester);
        double updateStudentSGPA(std::string roll, double sgpa);

        double calculateCGPA(std::string roll, int semester);
        double updateStudentCGPA(std::string roll, double cgpa);


};

#endif