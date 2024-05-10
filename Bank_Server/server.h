#ifndef MYSERVER_H
#define MYSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QMap>
#include <QHostAddress>
#include <QThread>

#include"DataBase.h"

class MyServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyServer(QObject *parent = 0);

signals:

private slots:
    //void newConnection();
    void ReadRequest();
public:
    /*Log in Function*/
    void processPostRequestlogin(QTcpSocket* socket, const QByteArray& requestData);
    bool searchInDatabase(const QString& name, QString& Password, QString& authority);

    /*Account Balance*/
    void processPostRequestviewaccountbalance(QTcpSocket* socket, const QByteArray& requestData);

    /*Get Account Number*/

    void processgetRequestgetaccountnumber(QTcpSocket* socket);

    /*Get Account Number Admin*/
    void processPostRequestgetaccountnumber(QTcpSocket* socket, const QByteArray& requestData);

    /*Make transaction*/
    void processPostRequestmaketransaction(QTcpSocket* socket, const QByteArray& requestData);

    /*Transfer amount*/
    void processPostRequesttransferamount(QTcpSocket* socket, const QByteArray& requestData);

    /*Get DataBase*/
    void processgetRequestgetbankdatabase(QTcpSocket* socket);

    /*Create User*/
    void processPostRequestcreateuser(QTcpSocket* socket, const QByteArray& requestData,QJsonObject& database);

    /*Update User*/
    void processPostRequestupdateuser(QTcpSocket* socket, const QByteArray& requestData,QJsonObject& database);

    /*send Response*/
    void sendResponse(QTcpSocket* socket, const QString& responseMessage);

    /*Delete User*/
    void processDeleteRequestdeleteuser(QTcpSocket* socket, const QByteArray& requestData,QJsonObject& database);

private:
    QTcpServer server;

protected:
    void incomingConnection(qintptr handle) override;
};

#endif
