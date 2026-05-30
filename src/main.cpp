#include <iostream>
#include "student_manager.h"
#include "auth_manager.h"

using namespace std;




int main() {

    cout << "WELCOME TO VIDYALAYA\n";
    cout << "Enter user id(roll_no) and password.\nNot a student ? Enter Name and password!\n";


    AuthManager auth;
    StudentManager manager(auth);

    int choice;


    while (true) {

        string username, password;
        cout << "Username : ";
        cin >> username;
        cout << "Password : ";
        cin >> password;

        string role = auth.login(username, password);

        if (role == "invalid") {

            cout << "Invalid Credentials!\n";
            return 0;
        }

        cout << "\nLogin Successful\n";
        cout << "Role : " << role << endl;

        cout << "Program started\n";
    
        bool loggedIn = true;


        while (loggedIn) {

            cout << "\n----------SMART STUDENT DATABASE---------\n";

            if (role == "student") {

                cout << "1.View my profile\n";
                cout << "2.LogOut\n";

                cout << "Enter a choice here : ";
            
                if (!(cin >> choice)) {

                    cin.clear();
                    cin.ignore(1000, '\n');

                    cout << "Invalid Input!\n";

                    continue;
                }

                switch (choice) {

                    case 1: {
                        string currRoll= auth.getStudentRoll(username);
                        manager.viewMyData(currRoll);
                        break;
                    }

                    case 2: 
                        loggedIn = false;
                        cout << "\n...Thank You...\n";
                        break;

                    default:
                        cout << "Invalid input!\n";
                }
            }


            else if (role == "teacher") {

                cout << "1. View Students\n";
                cout << "2. Search Students\n";
                cout << "3. Update Student\n";
                cout << "4. LogOut\n";

                cout << "Enter option here : ";
            
                if (!(cin >> choice)) {

                    cin.clear();
                    cin.ignore(1000, '\n');

                    cout << "Invalid Input!\n";

                    continue;
                }

                switch(choice) {
                    case 1:
                        manager.viewStudents();
                        break;

                    case 2:
                        manager.searchStudent();
                        break;

                    case 3:
                        manager.updateStudent();
                        break;

                    case 4:
                        loggedIn = false;
                        cout << "\n...Thank You...\n";
                        break;

                    default:
                        cout << "Invalid Input!\n";
                }
            }


            else if (role == "admin"){


                cout << "1. Add Student\n";
                cout << "2. View Students\n";
                cout << "3. Search Student\n";
                cout << "4. Delete Student\n";
                cout << "5. Update Student\n";
                cout << "6. Show Statistics\n";
                cout << "7. LogOut\n";

        
                cout << "Enter the choice number : ";
                if (!(cin >> choice)) {

                    cin.clear();
                    cin.ignore(1000, '\n');

                    cout << "Invalid Input!\n";

                    continue;
                }

                switch(choice) {
                    case 1:
                        manager.addStudent();
                        break;

                    case 2:
                        manager.viewStudents();
                        break;

                    case 3:
                        manager.searchStudent();
                        break;

                    case 4:
                        manager.deleteStudent();
                        break;

                    case 5:
                        manager.updateStudent();
                        break;

                    case 6:
                        manager.showStatistics();
                        break;

                    case 7:
                        cout << "\n...Thank You...\n";
                        loggedIn = false;
                        break;

                    default:
                        cout << "Invalid Choice\n";
                }
            }
        }
    }

    return 0;
}