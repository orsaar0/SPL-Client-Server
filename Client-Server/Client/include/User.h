//
// Created by saaro@wincs.cs.bgu.ac.il on 13/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_USER_H
#define BOOST_ECHO_CLIENT_USER_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <stdlib.h>
#include "BookClub.h"
#include "connectionHandler.h"
#include <mutex>
//#include <boost/lexical_cast.hpp>
//#include <boost/algorithm/string.hpp>
//#include <bits/stdc++.h>
//#include <boost/algorithm/string/split.hpp>
//#include <boost/algorithm/string/classification.hpp>

using namespace std;

class ConnectionHandler;

class BookClub;

class User {

public:
    User();

    ~User();

    int nextReceiptId();

    string &stringify(vector<pair<string, string>> &b);

    void clearHeaders(vector<pair<string, string>> &h);

    void login();

    void keyboardRun();

    void IOrun();

    void _login(vector<string> &a);

    void add(vector<string> &a);

    void join(vector<string> &a);

    void exit(vector<string> &a);

    void borrow(vector<string> &a);

//    void has(vector<string> &a);

    void _return(vector<string> &a);

//    void returning(vector<string> &a);

    void status(vector<string> &a);

//    void taking(vector<string> &a);

    void receipt(vector<string> &a);

    void logout();

    void terminateNow();

    void addReceipt(string command);

    void msgParse(string msgReceived);

    string getMsgReceiptById(int id);

    string getBookName(vector<string>& a, int begin, int end);

    vector<string> split(string, string);

    void sendFrame(vector<pair<string,string>>& a);

    ConnectionHandler *connectionHandler;
    BookClub *bookClub;
    int currReceiptId = (0);
private:
    std::mutex locker;
    vector<pair<string, string>> headers;
    vector<pair<string, string>> IOheaders;
    bool terminate = false;
    map<int, string> msgReceipt;
};


#endif //BOOST_ECHO_CLIENT_USER_H
