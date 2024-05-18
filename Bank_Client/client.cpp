#include "client.h"

Client::Client():activerequest(nullptr) {
    // Constructor implementation
    socket.connectToHost("192.168.1.8", 22);
}

void Client::setHttpRequestSender(HttpRequestSender* sender)
{
    activerequest = sender;

    activerequest->SetSocket(&socket); // to set the socket of the request
}

void Client::Log_in(QString User_name ,QString pass,QString authority)
{
    PostRequestSender* loginRequest = new PostRequestSender();

    setHttpRequestSender(loginRequest);

    qDebug()<<"Log in Function";

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["username"] =  User_name;
    jsonObject["password"] =  pass;
    jsonObject["authority"] = authority;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    loginRequest->sendRequest("post","http://192.168.1.8:22/postlogin",jsonData);

    delete loginRequest;

}

void Client::View_Account_Balance(QByteArray Account_Number)
{
    qDebug()<<"View Account Balance Function";

    PostRequestSender* viewBalanceRequest = new PostRequestSender();

    setHttpRequestSender(viewBalanceRequest);

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["Accountnumber"] =  QString::fromUtf8(Account_Number);
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    viewBalanceRequest->sendRequest("post","http://192.168.1.8:22/postviewaccountbalance",jsonData);

    delete viewBalanceRequest;
}

void Client::Get_Account_Number()
{
    qDebug()<<"Get Account Number Function";

    GetRequestSender* getAccountRequest = new GetRequestSender();

    setHttpRequestSender(getAccountRequest);

    getAccountRequest->sendRequest("get","http://192.168.1.8:22/getaccountnumber","");

    delete getAccountRequest;
}

void Client::Get_Account_Number(QString username)
{
    qDebug()<<"Get Account Number Function";

    PostRequestSender* GetAccountRequest = new PostRequestSender();

    setHttpRequestSender(GetAccountRequest);

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["username"] =  username;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    GetAccountRequest->sendRequest("post","http://192.168.1.8:22/postgetaccountnumber",jsonData);

    delete GetAccountRequest;
}

void Client::Make_Transaction(QByteArray accountnumber,int32_t transactionamount)
{
    qDebug()<<"Make Transaction Function";

    PostRequestSender* MakeTransactionRequest = new PostRequestSender();

    setHttpRequestSender(MakeTransactionRequest);

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["Accountnumber"] =  QString::fromUtf8(accountnumber);
    jsonObject["transactionamount"] = transactionamount;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    MakeTransactionRequest->sendRequest("post","http://192.168.1.8:22/postmaketransaction",jsonData);

    delete MakeTransactionRequest;
}

void Client::Transfer_Amount(QByteArray fromaccountnumber,QByteArray toaccountnumber,int32_t transactionamount)
{
    qDebug()<<"Transfer Amount Function";

    PostRequestSender* TransferAmountRequest = new PostRequestSender();

    setHttpRequestSender(TransferAmountRequest);

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["fromaccountnumber"] =  QString::fromUtf8(fromaccountnumber);
    jsonObject["toaccountnumber"] =  QString::fromUtf8(toaccountnumber);
    jsonObject["transactionamount"] = transactionamount;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    TransferAmountRequest->sendRequest("post","http://192.168.1.8:22/posttransferamount",jsonData);

    delete TransferAmountRequest;
}

void Client::View_Bank_DataBase()
{
    qDebug()<<"View Bank DataBase Function";

    GetRequestSender* viewDataBaseRequest = new GetRequestSender();

    setHttpRequestSender(viewDataBaseRequest);

    viewDataBaseRequest->sendRequest("get","http://192.168.1.8:22/getbankdatabase","");

    delete viewDataBaseRequest;
}

void Client::Create_User(QString User_Data)
{
    qDebug()<<"Create User Function";

    PostRequestSender* CreateUserRequest = new PostRequestSender();

    setHttpRequestSender(CreateUserRequest);

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

    CreateUserRequest->sendRequest("post","http://192.168.1.8:22/postcreateuser",jsonData);

    delete CreateUserRequest;
}

void Client::Update_User(QString Accountnumber,QString User_Data)
{
    qDebug()<<"Update User Function";

    PostRequestSender* UpdateUserRequest = new PostRequestSender();

    setHttpRequestSender(UpdateUserRequest);

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

    UpdateUserRequest->sendRequest("post","http://192.168.1.8:22/postupdateuser",jsonData);

    delete UpdateUserRequest;
}

void Client::Delete_User(QString Accountnumber)
{
    qDebug()<<"Delete User Function";

    DeleteRequestSender* DeleteUserRequest = new DeleteRequestSender();

    setHttpRequestSender(DeleteUserRequest);

    // Create a JSON object
    QJsonObject jsonObject;
    jsonObject["Accountnumber"] =  Accountnumber;
    QJsonDocument jsonDocument(jsonObject);

    // Convert the JSON object to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    DeleteUserRequest->sendRequest("delete","http://192.168.1.7:22/deleteuser",jsonData);

    delete DeleteUserRequest;
}


Client::~Client()
{
    delete activerequest;
}
