#include "activity.h"

#include <fstream>
#include <chrono>
#include <ctime>
#include <iostream>



using namespace std;




void Activity :: log(string message) {

    ofstream file("logs/activity.log", ios::app);

    time_t now = time(nullptr);
    string timestamp= ctime(&now);

    timestamp.pop_back();

    file << "[" << timestamp << "]" << message << endl;
}