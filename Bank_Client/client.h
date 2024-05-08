#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonDocument>
class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();

signals:

public slots:
    void connectToHost(QString hostname, quint16 port);
private slots:
    void Connected();
    void disconnected();
    void errorOccurred(QAbstractSocket::SocketError socketError);
    void hostFound();
    void stateChanged(QAbstractSocket::SocketState socketState);
    void disconnect();
    void readyRead();
public slots:
    void get(QString path);
    void post(QString path, QByteArray data);
    void put(QString path, QByteArray data);
    /*Log in function*/
    void Log_in(QByteArray User_name ,QByteArray pass,QByteArray authority);
    void View_Account_Balance(QByteArray Account_Number);
    void Get_Account_Number();
    void Get_Account_Number(QByteArray username);
    void Make_Transaction(QByteArray accountnumber,int32_t transactionamount);
    void Transfer_Amount(QByteArray fromaccountnumber,QByteArray toaccountnumber,int32_t transactionamount);
    void View_Bank_DataBase();
    void Create_User(QString User_Data);
    void Update_User(QString Accountnumber,QString User_Data);

private:
    QTcpSocket *socket;
};

#endif // CLIENT_H
