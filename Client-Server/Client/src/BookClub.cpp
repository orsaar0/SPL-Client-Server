//
// Created by saaro@wincs.cs.bgu.ac.il on 14/01/2020.
//
#include "../include/BookClub.h"

#include <unordered_map>

using namespace std;

BookClub::BookClub() : userName(), currTopicId(0), inventory(), topicId() {}


bool BookClub::hasBookAvailable(string genre, string bookName) {
    if (inventory.find(genre) == inventory.end())
        return false;
    else {
        for (Book book : inventory.at(genre)) {
            if (hasBook(genre, bookName) && book.getIsAvailable()) {
                return true;
            }
        }
        return false;
    }
}

/*
 * return true if we had the book and removed it.
 * false if we dont have the book
 */
bool BookClub::removeBook(string genre, string bookName) {
    if (!this->hasBookAvailable(genre, bookName)) {
        return false;
    } else {
        for (int i = 0; i < inventory.at(genre).size(); i++) {
            if (inventory.at(genre)[i].getBookName() == bookName) {
                inventory[genre][i].setIsAvailable(false);
                return true;
            }
        }
    }
}


void BookClub::addBook(string genre, string bookName, string borrowedFrom) {
    Book nBook(genre, bookName, borrowedFrom);
    if (inventory.find(genre) == inventory.end()) {
        vector<Book> vectorToAdd;
        vectorToAdd.push_back(nBook);
        inventory.insert({genre, vectorToAdd});
    } else {
        if (!this->hasBook(genre, bookName)) {
            getBookFromInv(genre, bookName).setIsAvailable(true);
            return;
        }
        inventory[genre].push_back(nBook);
    }
}

/*
 * receive genre name end return it's Id.
 * if doesnt exist, add it to the TopicId vector.
 */
string BookClub::getTopicId(string topic) {
    vector<pair<string, int>>::iterator it;
    for (it = topicId.begin(); it != topicId.end(); it++) {
        if (it.base()->first.compare(topic) == 0) {
            return to_string(it.base()->second);
        }
    }
    // add the new pair <topic, topicId>
    int next_id = nextTopicId();
    pair<string, int> newTopic(topic, next_id);
    topicId.push_back(newTopic);
    return to_string(next_id);
}

int BookClub::nextTopicId() {
    return currTopicId++;
}

/*
 * returns a string "userName: bookName1, bookname2...."
 */
string BookClub::printTopicInventory(string topic) {
    string toPrint = userName + ": ";
    for (auto t : inventory) {
        if (t.first == topic) {
            if (t.second.empty()) {
                return toPrint;
            }
            for (int i = 0; i < t.second.size() - 1; i++) {
                if (t.second[i].getIsAvailable())
                    toPrint += t.second[i].getBookName() + ", ";

            }
            if (t.second[t.second.size() - 1].getIsAvailable()) {
                toPrint += t.second[t.second.size() - 1].getBookName();
            }
            return toPrint;
        }
    }
    return toPrint;
}

string BookClub::getuserName() { return userName; }

bool BookClub::removeFromWishL(string bookName) {
    for (int i = 0; i < wishList.size(); i++) {
        if (wishList.at(i).getBookName().compare(bookName) == 0) {
            //  wishList.erase(wishList.begin() + i);
            wishList.at(i).setIsAvailable(false);
            return true;
        }
    }
    return false;
}

Book &BookClub::getBookFromInv(string genre, string bookName) {
    vector<Book> bookList = inventory.at(genre);
    for (Book book: bookList) {
        if (book.getBookName() == (bookName))
            return book;
    }
}

string BookClub::getBorrowedFrom(string genre, string bookName) {
    vector<Book> bookList = inventory.at(genre);
    for (Book book: bookList) {
        if (book.getBookName().compare(bookName) == 0)
            return book.getBorrowed_from();
    }
}


void BookClub::addToWishL(string genre, string bookName) {
    for (int i = 0; i < wishList.size(); i++) {
        if (wishList.at(i).getBookName().compare(bookName) == 0) {
            wishList.at(i).setIsAvailable(true);
            return;
        }

    }
    Book b(genre, bookName, "");
    wishList.push_back(b);
}

bool BookClub::hasBook(string topic, string bookName) {
    if (inventory.find(topic) != inventory.end()) {
        for (Book book : inventory[topic]) {
            if (book.getBookName() == bookName) {
                return true;
            }
        }
    }
    return false;
}

