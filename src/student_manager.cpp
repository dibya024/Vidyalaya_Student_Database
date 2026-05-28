#include <iostream>
#include "student_manager.h"

using namespace std;



StudentManager :: StudentManager(AuthManager& auth) : auth(auth){}

bool StudentManager :: addStudent() {

    string name, roll_no, branch;
    int rank, age;
    float sgpa, cgpa;


    string username, password, confpass;

    cout << "Create Username : ";
    cin >> username;

    if (username.empty()) {

        cout << "Username can't be empty!\n";
        return false;
    }

    cout << "Create Password : ";
    cin >> password;

    if (password.length() < 5) {

        cout << "Password is too short!\n";
        return false;
    }

    cout << "Confirm Password : ";
    cin >> confpass;

    if (password != confpass) {
        cout << "Password mismatched.\n";
        return false;
    }


    cin.ignore(1000, '\n');

    cout << "Enter Name :";
    getline(cin, name);

    cout << "Enter roll_no : ";
    cin >> roll_no;

    cin.ignore(1000, '\n');

    cout << "Enter Branch/Specialization : ";
    getline(cin, branch);

    cout << "Enter class rank :";
    cin >> rank;

    if (rank < 1) {
        cout << "Invalid rank!\n";
        return false;
    }

    cout << "Enter age : ";
    cin >> age;

    if (age <= 0) {
        cout << "Invalid age\n";
        return false;
    }

    cout << "Enter SGPA of this semester : ";
    cin >> sgpa;

    if (sgpa < 0 || sgpa > 10) {

        cout << "Invalid SGPA!\n";
        return false;
    }

    cout << "Enter CGPA : ";
    cin >> cgpa;

    if (cgpa < 0 || cgpa > 10) {

        cout << "Invalid CGPA!\n";
        return false;
    }

    Student s(name, roll_no, branch, rank, age, sgpa, cgpa);

    bool authSuccess = auth.registerStudent(username, password, roll_no);

    if (!authSuccess) {
        cout << "Login credentials creation failed!\n";
        return false;
    }


    bool dbSuccess = db.addStudent(s);

    if (!dbSuccess) {

        cout << "Student insertion failed!\n";
        return false;
    }

    cout << "New student details added successfully!\n";
    return true;

}




void StudentManager :: viewStudents() {

    db.viewStudents();
}




void StudentManager :: deleteStudent() {
    
    string sroll, sname;
    int option;

    cout << "1. Delete by name\n";
    cout << "2.Delete by roll_no\n";

    cout << "Enter an option : ";
    cin >> option;


    if (option == 1) {

        cin.ignore(1000, '\n');
        cout << "Enter the name of student you want to delete : ";
        getline(cin, sname);

        db.deleteByName(sname);
    }

    else if (option == 2) {

        cout << "Enter the roll_no you want to delete : ";
        cin >> sroll;

        db.deleteByRoll(sroll);
    }

    else{
        cout << "Invalid option\n";
        return;
    }


}





void StudentManager :: searchStudent() {
    
    string sroll, sname;
    int option;

    cout << "1. Search by name\n";
    cout << "2. Search by roll_no\n";

    cout << "Enter an option : ";
    cin >> option;


    if (option == 1) {

        cin.ignore(1000, '\n');
        cout << "Enter the name of student you want to search : ";
        getline(cin, sname);

        db.searchByName(sname);
    }

    else if (option == 2) {

        cout << "Enter the roll_no you want to search : ";
        cin >> sroll;

        db.searchByRoll(sroll);
    }

    else{
        cout << "Invalid option\n";
        return;
    }


}




void StudentManager :: updateStudent() {

    db.viewStudents();
    string sroll;
    int option;

    cout << "Enter the roll_no of the student : ";
    cin >> sroll;

    db.selectStudent(sroll);

    cout << "Enter an option from below which you want to update : ";
    cout << "1. Age\n";
    cout << "2. Rank\n";
    cout << "3. SGPA\n";
    cout << "4. CGPA\n";

    if (!(cin >> option)) {

        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input!\n";
        return;

    }

    string column;
    string value;


    switch (option) {
        case 1 : {
                 int age;
                 cout << "Enter new age\n";
                 cin >> age;

                 column= "age";
                 value= to_string(age);
                 break;
                 }

        case 2 : {
                 int rank;
                 cout << "Enter new rank\n";
                 cin >> rank;
                 
                 column= "rank";
                 value= to_string(rank);
                 break;
                 }

        case 3 : {
                 float sgpa;
                 cout << "Enter new SGPA\n";
                 cin >> sgpa;
                 
                 column= "sgpa";
                 value= to_string(sgpa);
                 break;
                 }

        case 4 : {
                 float cgpa;
                 cout << "Enter new CGPA\n";
                 cin >> cgpa;
                 
                 column= "cgpa";
                 value= to_string(cgpa);
                 break;
                 }

        default : cout << "Enter an option number!!\n";
                return;

                 
    }
    db.updateStudent(sroll, column, value);

}


void StudentManager :: viewMyData(string roll) {
    db.searchByRoll(roll);
}