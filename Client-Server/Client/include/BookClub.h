//
// Created by saaro@wincs.cs.bgu.ac.il on 14/01/2020.
//

#ifndef BOOST_ECHO_CLIENT_BOOKCLUB_H
#define BOOST_ECHO_CLIENT_BOOKCLUB_H

#include "../include/User.h"
#include "../include/Book.h"
#include <unordered_map>

using namespace std;

class BookClub {
public:
    BookClub();
    bool hasBookAvailable(string genre, string bookName);
    bool removeBook(string genre, string bookName);
    void addBook(string genre,string bookName,string ownerName);
    string getTopicId(string topic);
    int nextTopicId();
    string printTopicInventory(string topic);
    string getuserName();
    bool removeFromWishL(string bookName);
    void addToWishL(string genre, string bookName);
    Book& getBookFromInv(string genre, string bookName);
    string getBorrowedFrom(string gnere, string bookName);
    bool hasBook(string topic, string bookName);


    string userName;
    int currTopicId;
    unordered_map<string, vector<Book>> inventory;
    vector<pair<string, int>> topicId;
    vector<Book> wishList;

};

#endif //BOOST_ECHO_CLIENT_BOOKCLUB_H
