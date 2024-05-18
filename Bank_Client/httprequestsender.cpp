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
