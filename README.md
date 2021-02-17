# SPL-Client-Server
A server, which provides STOMP server services, and a client, which a user can use
in order to interact with the rest of the users. The server is implemented in Java and support both
Thread-Per-Client (TPS) and the Reactor, choosing which one according to arguments given on startup. The
client is implemented in C++ , and holds the required logic as described below.
All communication between the clients and the server are be according to the STOMP `Simple-Text-OrientedMessaging-Protocol' protocol.
