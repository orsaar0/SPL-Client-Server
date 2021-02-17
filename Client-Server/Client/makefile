CFLAGS:=-c -Wall -Weffc++ -g -std=c++11 -Iinclude -pthread
LDFLAGS:=-lboost_system -lpthread

all: StompBookClubClient
	@echo 'Building target: StompBookClubClient'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/StompBookClubClient bin/Book.o bin/BookClub.o bin/connectionHandler.o bin/main.o bin/User.o $(LDFLAGS)
	@echo 'Finished building target: StompBookClubClient'
	@echo ' '

StompBookClubClient: bin/Book.o bin/BookClub.o bin/connectionHandler.o bin/main.o bin/User.o $(LDFLAGS)


# Depends on the source and header files
bin/Book.o: src/Book.cpp
	g++ $(CFLAGS) -o bin/Book.o src/Book.cpp

bin/BookClub.o: src/BookClub.cpp
	g++ $(CFLAGS) -o bin/BookClub.o src/BookClub.cpp

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/main.o: src/main.cpp
	g++ $(CFLAGS) -o bin/main.o src/main.cpp

bin/User.o: src/User.cpp
	g++ $(CFLAGS) -o bin/User.o src/User.cpp
.PHONY: clean

#Clean the build directory
clean:
	rm -f bin/*