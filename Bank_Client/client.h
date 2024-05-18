<<<<<<< HEAD
#ifndef CLIENT_H
#define CLIENT_H
#include "socket.h"
#include "httprequestsender.h"

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class Client:public QObject {
    Q_OBJECT
private:
    Socket socket;
    HttpRequestSender* activerequest;

public:
    Client();
    ~Client();
    void setHttpRequestSender(HttpRequestSender* sender);

public slots:
    void Log_in(QString User_name ,QString pass,QString authority);
    void View_Account_Balance(QByteArray Account_Number);
    void Get_Account_Number();
    void Get_Account_Number(QString username);
    void Make_Transaction(QByteArray accountnumber,int32_t transactionamount);
    void Transfer_Amount(QByteArray fromaccountnumber,QByteArray toaccountnumber,int32_t transactionamount);
    void View_Bank_DataBase();
    void Create_User(QString User_Data);
    void Update_User(QString Accountnumber,QString User_Data);
    void Delete_User(QString Accountnumber);
};


#endif // CLIENT_H
=======
#ifndef CLIENT_H
#define CLIENT_H
#include "socket.h"
#include "httprequestsender.h"

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

class Client:public QObject {
    Q_OBJECT
private:
    Socket socket;
    HttpRequestSender* activerequest;

public:
    Client();
    ~Client();
    void setHttpRequestSender(HttpRequestSender* sender);

public slots:
    void Log_in(QString User_name ,QString pass,QString authority);
    void View_Account_Balance(QByteArray Account_Number);
    void Get_Account_Number();
    void Get_Account_Number(QString username);
    void Make_Transaction(QByteArray accountnumber,int32_t transactionamount);
    void Transfer_Amount(QByteArray fromaccountnumber,QByteArray toaccountnumber,int32_t transactionamount);
    void View_Bank_DataBase();
    void Create_User(QString User_Data);
    void Update_User(QString Accountnumber,QString User_Data);
    void Delete_User(QString Accountnumber);
};


#endif // CLIENT_H
>>>>>>> a55d8750151f0ad51a86ef31b98a3035fe87fe49
