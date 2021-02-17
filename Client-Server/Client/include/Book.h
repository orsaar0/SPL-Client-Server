//
// Created by itamarfi@wincs.cs.bgu.ac.il on 19/01/2020.
//
#include <string>

using namespace std;

class Book {
public:
    Book(string genre, string name, string borrowed_from);

    bool getIsAvailable();

    void setIsAvailable(bool availableStatus);

    string getBorrowed_from();

    string getBookName();

    string getBookOwner();


private:
    string name;
    string genre;
    string borrowed_from;
    bool isAvailable;

};