Banking system using the client-server model.\n
Application was developed using the QT/C++ framework.
The client-server interface is able to implement the following request/response pairs:Log
in, Get Account number, View transaction history, Transfer Amount and View Bank DB.
System was distributed on:
One Daemon applications (Server) - First VM (Linux Server) and windows as a client
Communication between two machines should happen via the TCP Sockets mechanism.
Usage of JSON as DB format.
The server was able to handle multiple requests from multiple clients using threads.
Each request should be executed in a separate thread until the request is handled.
