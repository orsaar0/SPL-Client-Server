//
// Created by saaro@wincs.cs.bgu.ac.il on 13/01/2020.
//


#include <thread>
#include "../include/User.h"
#include <mutex>


int main() {
    User *currUser = new User();
    currUser->login();
    mutex locker;
    // start the main loop
    std::thread keyboard(&User::keyboardRun, currUser);
    std::thread serverHandler(&User::IOrun, currUser);
    serverHandler.join();
    keyboard.join();
    delete (currUser);
    cout << "finish" << endl;
}

