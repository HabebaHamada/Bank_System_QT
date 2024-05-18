<<<<<<< HEAD
#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>

class Socket : public QObject
{
    Q_OBJECT
public:
    Socket();
    ~Socket();

signals:

public slots:
    void connectToHost(QString hostname, quint16 port);
public slots:
    void Connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void hostFound();
    void stateChanged(QAbstractSocket::SocketState socketState);
    void disconnect();
    void readyRead();

public:
    QTcpSocket *socket;
};

#endif // SOCKET_H
=======
#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>

class Socket : public QObject
{
    Q_OBJECT
public:
    Socket();
    ~Socket();

signals:

public slots:
    void connectToHost(QString hostname, quint16 port);
public slots:
    void Connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void hostFound();
    void stateChanged(QAbstractSocket::SocketState socketState);
    void disconnect();
    void readyRead();

public:
    QTcpSocket *socket;
};

#endif // SOCKET_H
>>>>>>> a55d8750151f0ad51a86ef31b98a3035fe87fe49
