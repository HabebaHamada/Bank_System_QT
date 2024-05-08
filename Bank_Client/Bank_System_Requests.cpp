#include"Bank_System_Requests.h"
#include"client.h"
#include"QJsonDocument"



void Client::Log_in(QByteArray User_name ,QByteArray pass,QByteArray authority)
{

    qDebug()<<"Log in Function";

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["username"] =  QString::fromUtf8(User_name);
    jsonObject["password"] =  QString::fromUtf8(pass);
    jsonObject["authority"] = QString::fromUtf8(authority);
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    post("http://192.168.1.7:22/postlogin",jsonData);
}

void Client::View_Account_Balance(QByteArray Account_Number)
{
    qDebug()<<"View Account Balance Function";

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["Accountnumber"] =  QString::fromUtf8(Account_Number);
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();


    post("http://192.168.1.7:22/postviewaccountbalance",jsonData);

}

void Client::Get_Account_Number()
{
    qDebug()<<"Get Account Number Function";

    get("http://192.168.1.7:22/getaccountnumber");
}

void Client::Get_Account_Number(QByteArray username)
{
    qDebug()<<"Get Account Number Function";

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["username"] =  QString::fromUtf8(username);
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();


    post("http://192.168.1.7:22/postgetaccountnumber",jsonData);
}

void Client::Make_Transaction(QByteArray accountnumber,int32_t transactionamount)
{
    qDebug()<<"Make Transaction Function";

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["Accountnumber"] =  QString::fromUtf8(accountnumber);
    jsonObject["transactionamount"] = transactionamount;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();


    post("http://192.168.1.7:22/postmaketransaction",jsonData);
}


void Client::Transfer_Amount(QByteArray fromaccountnumber,QByteArray toaccountnumber,int32_t transactionamount)
{
    qDebug()<<"Transfer Amount Function";

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["fromaccountnumber"] =  QString::fromUtf8(fromaccountnumber);
    jsonObject["toaccountnumber"] =  QString::fromUtf8(toaccountnumber);
    jsonObject["transactionamount"] = transactionamount;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();


    post("http://192.168.1.7:22/posttransferamount",jsonData);
}

void Client::View_Bank_DataBase()
{
    qDebug()<<"View Bank DataBase Function";

    get("http://192.168.1.7:22/getbankdatabase");
}

void Client::Create_User(QString User_Data)
{
    qDebug()<<"Create User Function";

    int usernameIndex = User_Data.indexOf("username:") + QString("username:").length();
    int usernameEndIndex = User_Data.indexOf(",", usernameIndex);
    QString username = User_Data.mid(usernameIndex, usernameEndIndex - usernameIndex);

    // Extracting the password
    int passwordIndex = User_Data.indexOf("password:") + QString("password:").length();
    int passwordEndIndex = User_Data.indexOf(",", passwordIndex);
    QString password = User_Data.mid(passwordIndex, passwordEndIndex - passwordIndex);

    // Extracting the account number
    int accountNumberIndex = User_Data.indexOf("accountnumber:") + QString("accountnumber:").length();
    QString accountNumber = User_Data.mid(accountNumberIndex);



    qDebug()<<username;
    qDebug()<<password;
    qDebug()<<accountNumber;

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["username"] = username;
    jsonObject["password"] =  password;
    jsonObject["Accountnumber"]=accountNumber;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    post("http://192.168.1.7:22/postcreateuser",jsonData);

}

void Client::Update_User(QString Accountnumber,QString User_Data)
{
    qDebug()<<"Update User Function";

    // Create a JSON object
    QJsonObject jsonObject;

    if(User_Data.contains("username:")&&User_Data.contains("password:"))
    {

    int usernameIndex = User_Data.indexOf("username:") + QString("username:").length();
    int usernameEndIndex = User_Data.indexOf(",", usernameIndex);
    QString username = User_Data.mid(usernameIndex, usernameEndIndex - usernameIndex);

    // Extracting the password
    int passwordIndex = User_Data.indexOf("password:") + QString("password:").length();
    QString password = User_Data.mid(passwordIndex);

    qDebug()<<username;
    qDebug()<<password;

    jsonObject["username"] = username;
    jsonObject["password"] =  password;
    }

    else if(User_Data.contains("username:"))
    {
    int usernameIndex = User_Data.indexOf("username:") + QString("username:").length();
    QString username = User_Data.mid(usernameIndex);
    qDebug()<<username;

    jsonObject["username"] = username;
    }
    else if(User_Data.contains("password:"))
    {
        // Extracting the password
        int passwordIndex = User_Data.indexOf("password:") + QString("password:").length();
        QString password = User_Data.mid(passwordIndex);
        qDebug()<<password;

        jsonObject["password"] = password;
    }


    jsonObject["Accountnumber"]=Accountnumber;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    post("http://192.168.1.7:22/postupdateuser",jsonData);

}
