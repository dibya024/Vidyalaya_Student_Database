#ifndef STUDENT_H
#define STUDENT_H

#include <string>


class Student{
    private:
        std::string name;
        std::string roll_no;
        std::string branch;

        int rank;
        int age;

        float sgpa;
        float cgpa;

    public:
        Student();
        Student (std::string name, std::string roll_no, std::string branch, int rank, int age, float sgpa, float cgpa);

        std::string getRoll() const;
        std::string getName() const;
        std::string getBranch() const;

        int getRank() const;
        int getAge() const;
        
        float getSgpa() const;
        float getCgpa() const;

        void display() const;

};



#endif