#include <iostream>
#include <iomanip>
#include "student.h"

using namespace std;

Student::Student() {   //Default constructor
    
    name= "";
    roll_no= "";
    branch= "";

    rank= 0;
    age= 0;

    sgpa= 0.0f;
    cgpa= 0.0f;

}

Student :: Student(string name, string roll_no, string branch, int rank, int age, float sgpa, float cgpa) {     //Parameterized Constructor

    this->name= name;                //this->object's member variable name= constructor's parameter name
    this->roll_no= roll_no;
    this->branch= branch;

    this->rank= rank;
    this->age= age;

    this->sgpa= sgpa;
    this->cgpa= cgpa;

}




string Student :: getName() const {
    return name;
}

string Student :: getRoll() const{
    return roll_no;
}

string Student :: getBranch() const {
    return branch;
}


int Student :: getRank() const {
    return rank;
}

int Student :: getAge() const {
    return age;
}


float Student :: getSgpa() const {
    return sgpa;
}

float Student :: getCgpa() const {
    return cgpa;
}

void Student :: display() const {

    cout << fixed << setprecision(2);

    cout << "\nName: " << name;
    cout << "\nRoll No.: " << roll_no;
    cout << "\nBranch: " << branch;

    cout << "\nRank: " << rank;
    cout << "\nAge: " << age;

    cout << "\nSGPA: " << sgpa;
    cout << "\nCGPA: " << cgpa << endl;
}