<<<<<<< HEAD
#include "httprequestsender.h"


HttpRequestSender::HttpRequestSender(Socket* sharedSocket) : Client_socket(sharedSocket) {
    // Constructor implementation
}

HttpRequestSender::HttpRequestSender() {
    // Constructor implementation
}
void HttpRequestSender::SetSocket(Socket *sharedSocket)
{
    Client_socket=sharedSocket;
}
=======
#include "httprequestsender.h"


HttpRequestSender::HttpRequestSender(Socket* sharedSocket) : Client_socket(sharedSocket) {
    // Constructor implementation
}

HttpRequestSender::HttpRequestSender() {
    // Constructor implementation
}
void HttpRequestSender::SetSocket(Socket *sharedSocket)
{
    Client_socket=sharedSocket;
}
>>>>>>> a55d8750151f0ad51a86ef31b98a3035fe87fe49
