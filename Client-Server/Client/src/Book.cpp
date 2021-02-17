
#include "../include/Book.h"

Book::Book(string genre, string name, string borrowed_from) :
        genre(genre), name(name), borrowed_from(borrowed_from), isAvailable(true) {
}

string Book::getBorrowed_from() {
    return borrowed_from;
}

bool Book::getIsAvailable() { return isAvailable; }

void Book::setIsAvailable(bool availableStatus) { isAvailable = availableStatus; }

string Book::getBookName() {
    return name;
}



