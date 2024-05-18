#include "socket.h"

Socket::Socket() {

    socket = new QTcpSocket;
    connect(socket,&QAbstractSocket::connected,this,&Socket::Connected);
    connect(socket,&QAbstractSocket::disconnected,this,&Socket::disconnected);
    connect(socket,&QAbstractSocket::hostFound,this,&Socket::hostFound);
    connect(socket,&QAbstractSocket::errorOccurred,this,&Socket::errorOccurred);
    connect(socket,&QAbstractSocket::stateChanged,this,&Socket::stateChanged);
    connect(socket,&QAbstractSocket::readyRead,this,&Socket::readyRead);

}

Socket:: ~Socket()
{
    delete socket;
}
void Socket::connectToHost(QString hostname, quint16 port)
{
    if(socket->isOpen())    disconnect();
    qInfo() << "Trying to connect to " << hostname << " on port " << port;
    socket->connectToHost(hostname, port);
}

void Socket::Connected()
{
    qInfo() << "Connected to Host";
}

void Socket::disconnected()
{
    qInfo() << "Connection Closed...";
}

void Socket::errorOccurred(QAbstractSocket::SocketError socketError)
{
    qInfo() << "Error: " << socketError << " " << socket->errorString();
}

void Socket::hostFound()
{
    qInfo() << "Server Host is Found";

}

void Socket::stateChanged(QAbstractSocket::SocketState socketState)
{
    qInfo() << "State: " << socketState;
}

void Socket::disconnect()
{
    socket->close();
    socket->waitForDisconnected();   //to make it synchronous
}

void Socket::readyRead()
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
}
