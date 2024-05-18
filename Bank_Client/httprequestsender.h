#ifndef HTTPREQUESTSENDER_H
#define HTTPREQUESTSENDER_H
#include "socket.h"

// Abstract base class for different HTTP operations
class HttpRequestSender
{
protected:
    Socket *Client_socket;
public:
    HttpRequestSender();
    HttpRequestSender(Socket* sharedSocket);
    void SetSocket(Socket* sharedSocket);
    virtual ~HttpRequestSender() {}
    virtual void sendRequest(const QString& method, const QString& path, const QByteArray& data) = 0;
};


// HTTP operation class for sending GET requests
class GetRequestSender : public HttpRequestSender
{
public:

    //Socket Get_Socket;

    explicit GetRequestSender() /*: HttpRequestSender(&Get_Socket)*/ {
       // Get_Socket.connectToHost("192.168.1.8", 22);

    }

    void sendRequest(const QString& method, const QString& path, const QByteArray& data) override
    {
         // Handle the response...
        if (Client_socket->socket->waitForConnected()) {
            // Send the GET request
            QString getRequest = "GET " + path + " HTTP/1.1\r\n"
                                                 "Content-Type: application/json\r\n"
                                                 "\r\n";
            Client_socket->socket->write(getRequest.toUtf8());

            // Wait for the response
            if (Client_socket->socket->waitForBytesWritten() && Client_socket->socket->waitForReadyRead()) {
                // Read the response
                //  QByteArray responseData = socket->readAll();
                //  qDebug() << "Response: " << responseData;
            }
        }

    }
};


// HTTP operation class for sending POST requests
class PostRequestSender : public HttpRequestSender
{
public:

   // Socket Post_Socket;

    explicit PostRequestSender() /*: HttpRequestSender(&Post_Socket)*/ {
       // Post_Socket.connectToHost("192.168.1.8", 22);

    }

    void sendRequest(const QString& method, const QString& path, const QByteArray& data) override
    {
        // Handle the response...
        if (Client_socket->socket->waitForConnected()) {
            // Construct the request headers
            QString requestHeaders = "POST " + path + " HTTP/1.1\r\n"
                                                      "Content-Type: application/json\r\n"
                                                      "Content-Length: " + QString::number(data.size()) + "\r\n"
                                                                      "\r\n";

            // Combine the request headers and data
            QByteArray requestData = requestHeaders.toUtf8() + data;

            // Send the request
            Client_socket->socket->write(requestData);

            // Wait for the response
            if (Client_socket->socket->waitForBytesWritten() && Client_socket->socket->waitForReadyRead()) {
                // Read the response
                // QByteArray responseData = socket->readAll();
                // qDebug() << "Response: " << responseData;
            }
        }

    }
};

// HTTP operation class for sending POST requests
class DeleteRequestSender : public HttpRequestSender
{
public:

    // Socket Delete_Socket;

    explicit DeleteRequestSender() /*: HttpRequestSender(&Delete_Socket)*/ {
        // Delete_Socket.connectToHost("192.168.1.8", 22);

    }

    void sendRequest(const QString& method, const QString& path, const QByteArray& data) override
    {
        // Handle the response...
        if (Client_socket->socket->waitForConnected()) {
            // Construct the request headers
            QString requestHeaders = "DELETE " + path + " HTTP/1.1\r\n"
                                                      "Content-Type: application/json\r\n"
                                                      "Content-Length: " + QString::number(data.size()) + "\r\n"
                                                                      "\r\n";

            // Combine the request headers and data
            QByteArray requestData = requestHeaders.toUtf8() + data;

            // Send the request
            Client_socket->socket->write(requestData);

            // Wait for the response
            if (Client_socket->socket->waitForBytesWritten() && Client_socket->socket->waitForReadyRead()) {
                // Read the response
                // QByteArray responseData = socket->readAll();
                // qDebug() << "Response: " << responseData;
            }
        }

    }
};


#endif // HTTPREQUESTSENDER_H

