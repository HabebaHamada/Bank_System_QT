#include "client.h"
#include <QObject>
Client::Client() {

    socket = new QTcpSocket;
    connect(socket,&QAbstractSocket::connected,this,&Client::Connected);
    connect(socket,&QAbstractSocket::disconnected,this,&Client::disconnected);
    connect(socket,&QAbstractSocket::hostFound,this,&Client::hostFound);
    connect(socket,&QAbstractSocket::errorOccurred,this,&Client::errorOccurred);
    connect(socket,&QAbstractSocket::stateChanged,this,&Client::stateChanged);
    connect(socket,&QAbstractSocket::readyRead,this,&Client::readyRead);

}

Client:: ~Client()
{
    delete socket;
}
void Client::connectToHost(QString hostname, quint16 port)
{
    if(socket->isOpen())    disconnect();
    qInfo() << "Trying to connect to " << hostname << " on port " << port;
    socket->connectToHost(hostname, port);
}

void Client::Connected()
{
    qInfo() << "Connected to Host";

   // QByteArray data;
   // data.append("Hello server , its me the client");
    //data.append("GET / HTTP/1.1\r\n");
    //data.append("Host: mshmae.pythonanywhere.com\r\n");
    // data.append("Connection: Close \r\n");
    //data.append("\r\n");

   // socket->write(data);
    //socket->flush();

    //socket->waitForBytesWritten(3000);
    //socket->close();
}

void Client::disconnected()
{
    qInfo() << "Connection Closed...";
}

void Client::errorOccurred(QAbstractSocket::SocketError socketError)
{
    qInfo() << "Error: " << socketError << " " << socket->errorString();
}

void Client::hostFound()
{
    qInfo() << "Server Host is Found";

}

void Client::stateChanged(QAbstractSocket::SocketState socketState)
{
    qInfo() << "State: " << socketState;
}

void Client::disconnect()
{
    socket->close();
    socket->waitForDisconnected();   //to make it synchronous
}

void Client::readyRead()
{
    qInfo() << "Reading Data from Server";
    QByteArray data= socket->readAll();
    QTextStream in(&data);
    while(!in.atEnd())
    {
        QString line= in.readLine();
        line.remove("\n");
        line.remove("\r");
        qDebug().noquote()<<line;
    }
   // qInfo() << "Data: " << socket->readAll();
}

void Client::get(QString path)
{
    if (socket->waitForConnected()) {
        // Send the GET request
        QString getRequest = "GET " + path + " HTTP/1.1\r\n"
                                             "Content-Type: application/json\r\n"
                                             // "Host: " + serverAddress + "\r\n"
                                             //   "Connection: close\r\n"
                                             "\r\n";
        socket->write(getRequest.toUtf8());

        // Wait for the response
        if (socket->waitForBytesWritten() && socket->waitForReadyRead()) {
            // Read the response
            //  QByteArray responseData = socket->readAll();
            //  qDebug() << "Response: " << responseData;
        }
    }
   // connect(socket,&QAbstractSocket::readyRead, this, &Client::readyRead);
}

void Client::post(QString path, QByteArray data)
{
    if (socket->waitForConnected()) {
        // Construct the request headers
        QString requestHeaders = "POST " + path + " HTTP/1.1\r\n"
                                                  //  "Host: " + serverAddress + "\r\n"
                                                  "Content-Type: application/json\r\n"
                                                  "Content-Length: " + QString::number(data.size()) + "\r\n"
                                                                  //  "Connection: close\r\n"
                                                                  "\r\n";

        // Combine the request headers and data
        QByteArray requestData = requestHeaders.toUtf8() + data;

        // Send the request
        socket->write(requestData);

        // Wait for the response
        if (socket->waitForBytesWritten() && socket->waitForReadyRead()) {
            // Read the response
            // QByteArray responseData = socket->readAll();
            // qDebug() << "Response: " << responseData;
        }
    }
   // connect(socket,&QAbstractSocket::readyRead, this, &Client::readyRead);

}

void Client::put(QString path, QByteArray data)
{
    if (socket->waitForConnected()) {
        // Construct the request headers
        QString requestHeaders = "PUT " + path + " HTTP/1.1\r\n"
                                                  //  "Host: " + serverAddress + "\r\n"
                                                  "Content-Type: application/json\r\n"
                                                  "Content-Length: " + QString::number(data.size()) + "\r\n"
                                                                  //  "Connection: close\r\n"
                                                                  "\r\n";

        // Combine the request headers and data
        QByteArray requestData = requestHeaders.toUtf8() + data;

        // Send the request
        socket->write(requestData);

        // Wait for the response
        if (socket->waitForBytesWritten() && socket->waitForReadyRead()) {
            // Read the response
            // QByteArray responseData = socket->readAll();
            // qDebug() << "Response: " << responseData;
        }
    }
   // connect(socket,&QAbstractSocket::readyRead, this, &Client::readyRead);
}
