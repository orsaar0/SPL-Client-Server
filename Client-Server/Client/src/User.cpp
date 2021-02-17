//
// Created by saaro@wincs.cs.bgu.ac.il on 13/01/2020.
//


#include <include/User.h>

using namespace std;

User::User() = default;

User::~User() {
    delete connectionHandler;
    delete bookClub;
}

// region encode
/*
 *  the following static functions are the parsing functions of the different messages came from keyboard.
 */

void User::_login(vector<string> &a) {
    bookClub->userName = a[2];
    headers.emplace_back("CONNECT", "");
    headers.emplace_back("accept-version", "1.2");
    vector<string> host_port = split(a[1], ":");
    headers.emplace_back("host", host_port[0]);
    headers.emplace_back("login", a[2]);
    headers.emplace_back("passcode", a[3]);
}

void User::add(vector<string> &a) {
    headers.emplace_back("SEND", "");
    headers.emplace_back("destination", a[1]);
    headers.emplace_back("\n", bookClub->userName + " has added the book " + getBookName(a, 2, a.size() - 2));
    bookClub->addBook(a[1], getBookName(a, 2, a.size() - 2), "");
    bookClub->removeFromWishL(getBookName(a, 2, a.size() - 2));
}

void User::join(vector<string> &a) {
    headers.emplace_back("SUBSCRIBE", "");
    headers.emplace_back("destination", a[1]);
    headers.emplace_back("id", bookClub->getTopicId(a[1]));
    headers.emplace_back("receipt", to_string(nextReceiptId()));
}

void User::exit(vector<string> &a) {
    headers.emplace_back("UNSUBSCRIBE", "");
    headers.emplace_back("destination", a[1]);
    headers.emplace_back("id", bookClub->getTopicId(a[1]));
    string receiptId = to_string(nextReceiptId());
    headers.emplace_back("receipt", receiptId);
}

void User::borrow(vector<string> &a) {
    headers.emplace_back("SEND", "");
    headers.emplace_back("destination", a[1]);
    headers.emplace_back("\n", bookClub->userName + " wish to borrow " + a[2]);
    bookClub->addToWishL(a[1], a[2]);
}

void User::_return(vector<string> &a) {
    headers.emplace_back("SEND", "");
    headers.emplace_back("destination", a[1]);
    headers.emplace_back("\n", "Returning " + a[2] + " to " + bookClub->getBorrowedFrom(a[1], a[2]));
}

void User::status(vector<string> &a) {
    headers.emplace_back("SEND", "");
    headers.emplace_back("destination", a[1]);
    headers.emplace_back("\n", "book status");
}

void User::receipt(vector<string> &a) {
    IOheaders.emplace_back("RECEIPT", "");
    vector<string> vectorizedReceipt = split(a[1], ":");
    IOheaders.emplace_back(vectorizedReceipt[0], vectorizedReceipt[1]);
}

void User::logout() {
    headers.emplace_back("DISCONNECT", "");
    headers.emplace_back("receipt", to_string(nextReceiptId()));
}
// endregion

void User::login() {
    cout << "Please Enter 'login {host}:{port} {userName} {passcode}'" << endl;
    bool terminated = false;
    while (!terminated) {
        string msg;
        getline(cin, msg);
        vector<string> s = split(msg, " ");
        if (s[0] == ("login")) {
            bookClub = new BookClub();
            bookClub->userName = s[2];
            // init the connection handler
            vector<string> host_port = split(s[1], ":");
            connectionHandler = new ConnectionHandler(host_port[0], (short) stol(host_port[1]));
            if (connectionHandler->connect()) {
                terminated = true;
                // send the CONNECT frame
                _login(s);
                sendFrame(headers);
                // clear headers for reuse
                clearHeaders(headers);
            }
        } else {
            cout << "ERROR: you need to login first" << endl;
        }
    }
}

void User::keyboardRun() {
    while (!terminate) {
        string userInput;
        getline(cin, userInput);
        bool send = true;
        vector<string> msg = split(userInput, " ");
        // make and send the correct parsing according to the protocol
        if (msg[0] == ("add")) {
            add(msg);
        } else if (msg[0] == ("join")) {
            join(msg);
            string toJoin = headers[1].second;
            addReceipt("Joined club " + toJoin);
        } else if (msg[0] == ("exit")) {
            exit(msg);
            string toJoin = headers[1].second;
            addReceipt("Exited club " + toJoin);
        } else if (msg[0] == ("borrow")) {
            borrow(msg);
        } else if (msg[0] == ("return")) {
            _return(msg);
            bookClub->removeBook(msg[1], msg[2]);
        } else if (msg[0] == ("status")) {
            status(msg);
        }
            // if disconnected -> out of the loop, back to main func
        else if (msg[0] == ("logout") || msg[0] == ("bye")) {
            logout();
            addReceipt("DISCONNECT");
        }
            // if invalid request
        else {
            cout << "invalid request" << endl;
            send = false;
        }

        if (send) {
            // send the msg to server, thread safe
            sendFrame(headers);

            // clear the headers, ready for next msg
            clearHeaders(headers);
        }

    }
}

void User::msgParse(string msgReceived) {
    vector<string> vectorizedMsg = split(msgReceived, "\n");
    IOheaders.emplace_back(vectorizedMsg[0], "\n");
    for (int i = 1; i < 4; i++) {
        if (vectorizedMsg[i] != "\n") {
            vector<string> pair = split(vectorizedMsg[i], ":");
            IOheaders.emplace_back(pair[0], pair[1]);
        }
    }
    IOheaders.emplace_back("\n\n", vectorizedMsg[5]);
}

void User::IOrun() {
    vector<string> vectorizedMsg;

    while (!terminate) {
        string msgRecieved;
        connectionHandler->getFrameAscii(msgRecieved, '\0');
        cout << msgRecieved << endl;

        vectorizedMsg = split(msgRecieved, "\n");
        if (vectorizedMsg[0] == ("ERROR")) {
            cout << msgRecieved << endl;
            terminateNow();
            connectionHandler->close();

        } else if (vectorizedMsg[0] == ("CONNECTED")) {
            cout << "Login Successful" << endl;

        } else if (vectorizedMsg[0] == ("RECEIPT")) {
            receipt(vectorizedMsg);
            int id = stoi(IOheaders[1].second);
            string msg = getMsgReceiptById(id);
            if (msg == "DISCONNECT") {
                terminateNow();
                connectionHandler->close();
            }
            cout << msg << endl;

        } else if (vectorizedMsg[0] == ("MESSAGE")) {
            // init IOheaders with all the info of the MESSAGE
            msgParse(msgRecieved);
            // init the frameBody
            vector<string> frameBody = split(IOheaders[4].second, " ");
            string topic = IOheaders[3].second;
            // only if the massage is 'returning' or 'taking' **and** related to me ->
            if ((frameBody[0] == "Returning" || frameBody[0] == "Taking") &&
                (frameBody[frameBody.size() - 1] == bookClub->getuserName())) {

                string bookName = getBookName(frameBody, 1, frameBody.size() - 3);
                // set Book is Available if the message is 'returning'
                if (frameBody[0] == "Returning") {
                    bookClub->addBook(topic, bookName, "dosen't matter");
                }
                if (frameBody[0] == "Taking") {
                    bookClub->removeBook(topic, bookName);
                }

                // if the message is 'has' or 'wish' **and** its not me!
            } else if ((frameBody[1] == "has" || frameBody[1] == "wish") && (bookClub->getuserName() != frameBody[0]) &&
                       (frameBody[2] != "added")) {
                clearHeaders(IOheaders);
                if (frameBody[1] == "has") {
                    string bookName = getBookName(frameBody, 2, frameBody.size() - 2);
                    //if the book is in my wishlist
                    if (bookClub->removeFromWishL(bookName)) { // return false if the book is not in my wishlist
                        bookClub->addBook(topic, bookName, frameBody[0]);
                        IOheaders.emplace_back("SEND", "");
                        IOheaders.emplace_back("destination", topic);
                        IOheaders.emplace_back("\n", "Taking " + bookName + " from " + frameBody[0]);
                        sendFrame(IOheaders);
                    }
                } else {
                    string bookName = getBookName(frameBody, 4, frameBody.size() - 4);
                    // if i have the book to borrow
                    if (bookClub->hasBookAvailable(IOheaders[3].second, bookName)) {
                        IOheaders.emplace_back("SEND", "");
                        IOheaders.emplace_back("destination", topic);
                        IOheaders.emplace_back("\n", bookClub->userName + " has " + bookName);
                        sendFrame(IOheaders);
                    }
                }

                // status should be done anyway
            } else if (frameBody[1] == "status") {
                clearHeaders(IOheaders);
                IOheaders.emplace_back("SEND", "");
                IOheaders.emplace_back("destination", topic);
                IOheaders.emplace_back("\n", bookClub->printTopicInventory(topic));
                sendFrame(IOheaders);
            }
        }
        clearHeaders(IOheaders);
    }
}

int User::nextReceiptId() { return currReceiptId = currReceiptId + 1; }

/*
 * get a STOMP Frame and return the frame into one string
 */
string &User::stringify(vector<pair<string, string>> &b) {
    vector<pair<string, string>>::iterator it;

    // put the title first
    string &stringMsg = b.begin().base()->first;
    stringMsg += "\n";
    for (it = b.begin() + 1; it < b.end(); it++) {
        if (it.base()->first == "\n") {
            stringMsg += "\n" + it.base()->second + "\n";
            return stringMsg;
        }
        stringMsg += it.base()->first + ":" + it.base()->second + "\n";
    }
    stringMsg += "\n";
    return stringMsg;
}

/*
 *  clear the pair vector field "headers"/IOheaders
 */
void User::clearHeaders(vector<pair<string, string>> &headers) {
    headers.erase(headers.begin(), headers.end());
}

void User::terminateNow() { terminate = true; }

void User::addReceipt(string command) {
    msgReceipt.insert({currReceiptId, command});
}

string User::getMsgReceiptById(int id) {
    return msgReceipt.at(id);
}

vector<string> User::split(string s, string delimiter) {
    vector<string> list;
    size_t pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != string::npos) {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}

string User::getBookName(vector<string> &a, int begin, int end) {
    string bookName = a[begin];
    for (int i = begin + 1; i <= end; i++) {
        bookName = bookName + " " + a[i];
    }
    return bookName;
}

void User::sendFrame(vector<pair<string, string>> &a) {
    locker.lock();
    connectionHandler->sendFrameAscii(stringify(a), '\0');
    locker.unlock();
}




