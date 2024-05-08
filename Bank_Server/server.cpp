#include "server.h"
#include"DataBase.h"
struct ClientData
{
    QString name;
    QString authority;
};

QByteArray Data_Base;
QJsonObject userTable;
QMap<QTcpSocket *,ClientData> clientmap;

MyServer::MyServer(QObject *parent) :
    QObject(parent)
{
    Data_Base=Create_DataBase(userTable);

    //server = new QTcpServer(this);
    connect(&server, &QTcpServer::newConnection, this, &MyServer::newConnection);

    if(!server.listen(QHostAddress::Any, 22))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void MyServer::newConnection()
{
    QTcpSocket *socket = server.nextPendingConnection();

    connect(socket, &QTcpSocket::readyRead, this, &MyServer::ReadRequest);

    qDebug()<<"new connection";


    // Disconnect the client
    //socket->disconnectFromHost();
    //socket->deleteLater();

    //socket->write("hello client\r\n");
    //socket->flush();

    //socket->waitForBytesWritten(3000);

    //socket->close();
}

void MyServer::ReadRequest()
{

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    QByteArray requestData = socket->readAll();
    qDebug() << "Client request: " << requestData;

    QString requeststring(requestData);

    if(requeststring.startsWith("GET"))
    {
        // Send a response to the client
       /* QByteArray response = "HTTP/1.0 200 OK\r\n\r\n";
        response+="Hello client this is a GET request";
        socket->write(response);*/
        if(requeststring.contains("/getaccountnumber")){
            processgetRequestgetaccountnumber(socket);
        }
        else if(requeststring.contains("/getbankdatabase")){
            processgetRequestgetbankdatabase(socket);
        }
    }
    else if(requeststring.startsWith("POST"))
    {
        if(requeststring.contains("/postlogin")){
            processPostRequestlogin(socket,requestData);
        }
        else if(requeststring.contains("/postviewaccountbalance")){
            processPostRequestviewaccountbalance(socket,requestData);
        }
        else if(requeststring.contains("/postgetaccountnumber")){
            processPostRequestgetaccountnumber(socket,requestData);
        }
        else if(requeststring.contains("/postmaketransaction")){
            processPostRequestmaketransaction(socket,requestData);
        }
       else if(requeststring.contains("/posttransferamount"))
        {
            processPostRequesttransferamount(socket,requestData);
        }
        else if(requeststring.contains("/postcreateuser"))
        {
            processPostRequestcreateuser(socket,requestData,userTable);
        }
        else if(requeststring.contains("/postupdateuser"))
        {
            processPostRequestupdateuser(socket,requestData,userTable);
        }
    }
    else if(requeststring.startsWith("PUT"))
    {
        // Send a response to the client
        QByteArray response = "HTTP/1.0 200 OK\r\n\r\n";
        response+="Hello client this is a PUT request";
        socket->write(response);
    }

}


void MyServer::processPostRequestlogin(QTcpSocket* socket, const QByteArray& requestData)
{
    // Extract the JSON data from the request
    QByteArray jsonBytes = requestData.mid(requestData.indexOf('{'));

    QByteArray response;
    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        // Process the jsonObject as needed
        qDebug() << "Received POST request with data:";
        qDebug() << "Name:" << jsonObject["username"].toString();
        qDebug() << "Password:" << jsonObject["password"].toString();
        qDebug() << "Authority:" << jsonObject["authority"].toString();

        QString name      = jsonObject["username"].toString();
        QString password  = jsonObject["password"].toString();
        QString Authority = jsonObject["authority"].toString();


        // Search for the data in the database
        bool dataExists = searchInDatabase(name, password,Authority);

        // Send a response to the client

        if (dataExists) {

            clientmap[socket].name=name;
            clientmap[socket].authority=Authority;

            qDebug()<<socket;
            qDebug()<<clientmap[socket].name;
            qDebug()<<clientmap[socket].authority;

            response = "HTTP/1.0 200 OK\r\n\r\n";
            response+= "Logged in successfully";

        } else {
            response = "HTTP/1.0 200 OK\r\n\r\n";
            response+= "user not found";
        }
    }


    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
}

bool MyServer::searchInDatabase(const QString& name, QString& Password, QString& authority)
{
    QFile databaseFile("DataBase.json");
    if (!databaseFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open database file";
        return false;
    }

    QByteArray databaseData = databaseFile.readAll();
    databaseFile.close();

    QJsonDocument jsonDoc = QJsonDocument::fromJson(databaseData);
    if (!jsonDoc.isObject()) {
        qDebug() << "Invalid database file format";
        return false;
    }

    QJsonObject jsonObject = jsonDoc.object();
    QJsonArray dataArray = jsonObject.value("users").toArray();

    for (const QJsonValue& value : dataArray) {
        QJsonObject dataObject = value.toObject();
        QString dataName = dataObject.value("username").toString();
        QString datapassword = dataObject.value("password").toString();
        QString dataAuthority = dataObject.value("authority").toString();

        if (dataName == name && datapassword== Password && dataAuthority==authority) {
            qDebug() << "Data found";
            return true; // Data found in the database
        }
    }

    qDebug() << "Data not found";

    return false; // Data not found in the database
}

void MyServer::processPostRequestviewaccountbalance(QTcpSocket* socket, const QByteArray& requestData)
{
    qDebug()<<"processPostRequestviewaccountbalance";

    // Extract the JSON data from the request
    QByteArray jsonBytes = requestData.mid(requestData.indexOf('{'));

    QByteArray response;

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    if (!jsonDoc.isNull() && jsonDoc.isObject()) {
        QJsonObject jsonObject = jsonDoc.object();

        // Process the jsonObject as needed
        qDebug() << "Received POST request with data:";
        qDebug() << "Account Number:" << jsonObject["Accountnumber"].toString();

        QString AccountNumber  = jsonObject["Accountnumber"].toString();


        QFile databaseFile("DataBase.json");
        if (!databaseFile.open(QIODevice::ReadOnly)) {
            qDebug() << "Failed to open database file";
        }

        QByteArray databaseData = databaseFile.readAll();
        databaseFile.close();

        QJsonDocument jsonDoc = QJsonDocument::fromJson(databaseData);
        if (!jsonDoc.isObject()) {
            qDebug() << "Invalid database file format";
        }

        QJsonObject jsonObject_search = jsonDoc.object();
        QJsonArray dataArray = jsonObject_search.value("users").toArray();

        bool dataExists=false;
        int32_t dataAccountbalance;
        for (const QJsonValue& value : dataArray) {
            QJsonObject dataObject = value.toObject();
            QString dataAccountNum = dataObject.value("Accountnumber").toString();

            if (dataAccountNum == AccountNumber) {
                qDebug() << "Data found";   // Data found in the database
                dataAccountbalance = dataObject.value("balance").toInt();
                dataExists=true;
                break;
            }
        }


        // Send a response to the client

        if (dataExists) {
            response = "HTTP/1.0 200 OK\r\n\r\n";
            response+= "Account Balance:";
            response+= QString::number(dataAccountbalance).toUtf8();

        } else {
            response = "HTTP/1.0 200 OK\r\n\r\n";
            response+= "Account Number not found";
        }
    }


    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
}

void MyServer::processgetRequestgetaccountnumber(QTcpSocket* socket)
{

    qDebug()<<"processgetRequestgetaccountnumber";

    QByteArray response;
    //check if this socket is in the qmap
    //search for this specific socket in q map and make sure that its value(name) is the given user name
    // then make sure that this name is a user in the first place
    // if all of this is true then get its account number
    if(clientmap.contains(socket))
    {

        QString name=clientmap.value(socket).name;

        qDebug()<<name;

            QFile databaseFile("DataBase.json");
            if (!databaseFile.open(QIODevice::ReadOnly)) {
                qDebug() << "Failed to open database file";
            }

            QByteArray databaseData = databaseFile.readAll();
            databaseFile.close();

            QJsonDocument jsonDoc = QJsonDocument::fromJson(databaseData);
            if (!jsonDoc.isObject()) {
                qDebug() << "Invalid database file format";
            }

            QJsonObject jsonObject_search = jsonDoc.object();
            QJsonArray dataArray = jsonObject_search.value("users").toArray();

            QString dataAccountnumber;
            for (const QJsonValue& value : dataArray) {
                QJsonObject dataObject = value.toObject();
                QString datausername = dataObject.value("username").toString();
                QString datauserrole = dataObject.value("authority").toString();

                if (datausername == name) {
                    if(datauserrole=="standard")
                    {
                    qDebug() << "Data found";   // Data found in the database
                    dataAccountnumber = dataObject.value("Accountnumber").toString();
                    response = "HTTP/1.0 200 OK\r\n\r\n";
                    response+= "Account Number:";
                    response+= dataAccountnumber.toUtf8();
                    break;
                    }
                    else
                    {
                        response = "HTTP/1.0 200 OK\r\n\r\n";
                        response+= "Not Authorized";
                    }
                }

        }

    }
    else
    {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response+= "Account Number: :";
    }


    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();

}

void MyServer::processPostRequestgetaccountnumber(QTcpSocket* socket, const QByteArray& requestData)
{
    qDebug() << "processposttRequestgetaccountnumber";

    QByteArray response;

    if (!clientmap.contains(socket)) {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Logged in Before, Please Log in First";
        response += ", Account Number:  ";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    QString autho = clientmap.value(socket).authority;
    qDebug() << autho;

    if (autho != "admin") {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Authorized";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    // Extract the JSON data from the request
    QByteArray jsonBytes = requestData.mid(requestData.indexOf('{'));

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data";
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();

    // Process the jsonObject as needed
    qDebug() << "Received POST request with data:";
    qDebug() << "Username:" << jsonObject["username"].toString();

    QString name = jsonObject["username"].toString();

    QFile databaseFile("DataBase.json");
    if (!databaseFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open database file";
        return;
    }

    QByteArray databaseData = databaseFile.readAll();
    databaseFile.close();

    QJsonDocument databaseDoc = QJsonDocument::fromJson(databaseData);
    if (!databaseDoc.isObject()) {
        qDebug() << "Invalid database file format";
        return;
    }

    QJsonObject databaseObject = databaseDoc.object();
    QJsonArray dataArray = databaseObject.value("users").toArray();

    QString dataAccountnumber;
    bool dataFound = false;

    for (const QJsonValue& value : dataArray) {
        QJsonObject dataObject = value.toObject();
        QString dataUsername = dataObject.value("username").toString();

        if (dataUsername == name) {
            qDebug() << "Data found";   // Data found in the database
            dataAccountnumber = dataObject.value("Accountnumber").toString();
            response = "HTTP/1.0 200 OK\r\n\r\n";
            response += "Account Number: " + dataAccountnumber.toUtf8();
            dataFound = true;
            break;
        }
    }

    if (!dataFound) {
        qDebug() << "Data not found in the database";
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Data not found in the database";
    }

    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
}


void MyServer::processPostRequestmaketransaction(QTcpSocket* socket, const QByteArray& requestData)
{
    qDebug() << "processPostRequestmaketransaction";

    QByteArray response;

    if (!clientmap.contains(socket)) {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Logged in Before, Please Log in First";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    QString autho = clientmap.value(socket).authority;
    qDebug() << autho;

    if (autho != "standard") {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Authorized";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    // Extract the JSON data from the request
    QByteArray jsonBytes = requestData.mid(requestData.indexOf('{'));

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data";
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();

    // Process the jsonObject as needed
    qDebug() << "Received POST request with data:";
    qDebug() << "Accountnumber:" << jsonObject["Accountnumber"].toString();
    qDebug() << "Transaction amount:" << jsonObject["transactionamount"].toInt();

    QString accountnumber = jsonObject["Accountnumber"].toString();
    int32_t transactionAmount = jsonObject["transactionamount"].toInt();

    QFile databaseFile("DataBase.json");
    if (!databaseFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open database file";
        return;
    }

    QByteArray databaseData = databaseFile.readAll();
    databaseFile.close();

    QJsonDocument databaseDoc = QJsonDocument::fromJson(databaseData);
    if (!databaseDoc.isObject()) {
        qDebug() << "Invalid database file format";
        return;
    }

    QJsonObject databaseObject = databaseDoc.object();
    QJsonArray dataArray = databaseObject.value("users").toArray();

    int32_t balance = 0;
    bool transactionDone = false;

    for (const QJsonValue& value : dataArray) {
        QJsonObject dataObject = value.toObject();
        QString dataAccountnumber = dataObject.value("Accountnumber").toString();

        if (dataAccountnumber == accountnumber) {
            qDebug() << "Data found";   // Data found in the database
            balance = dataObject.value("balance").toInt();
            int32_t newBalance = balance + transactionAmount;

            if (newBalance < 0) {
                response = "HTTP/1.0 200 OK\r\n\r\n";
                response += "False, Transaction can't be done, New Balance is: " + QString::number(balance).toUtf8();
                transactionDone = true;
                break;
            }

            dataObject["balance"] = newBalance;
            qDebug() << dataObject.value("balance").toInt();

            response = "HTTP/1.0 200 OK\r\n\r\n";
            response += "true Transaction is done, New Balance is: " + QString::number(newBalance).toUtf8();
            transactionDone = true;
            break;
        }
    }

    if (!transactionDone) {
        qDebug() << "Transaction data not found in the database";
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Transaction data not found in the database";
    }

    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
}

void MyServer::processPostRequesttransferamount(QTcpSocket* socket, const QByteArray& requestData)
{
    qDebug() << "processPostRequesttransferamount";

    QByteArray response;

    if (!clientmap.contains(socket)) {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "not Logged in Before, Please Log in First";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    QString autho = clientmap.value(socket).authority;
    qDebug() << autho;

    if (autho != "standard") {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Authorized";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    // Extract the JSON data from the request
    QByteArray jsonBytes = requestData.mid(requestData.indexOf('{'));

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data";
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();

    // Process the jsonObject as needed
    qDebug() << "Received POST request with data:";
    qDebug() << "Accountnumber:" << jsonObject["fromaccountnumber"].toString();
    qDebug() << "Transaction amount:" << jsonObject["transactionamount"].toInt();

    QString fromaccountnumber = jsonObject["fromaccountnumber"].toString();
    QString toaccountnumber = jsonObject["toaccountnumber"].toString();
    int32_t transactionAmount = jsonObject["transactionamount"].toInt();

    QFile databaseFile("DataBase.json");
    if (!databaseFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open database file";
        return;
    }

    QByteArray databaseData = databaseFile.readAll();
    databaseFile.close();

    QJsonDocument databaseDoc = QJsonDocument::fromJson(databaseData);
    if (!databaseDoc.isObject()) {
        qDebug() << "Invalid database file format";
        return;
    }

    QJsonObject databaseObject = databaseDoc.object();
    QJsonArray dataArray = databaseObject.value("users").toArray();

    int32_t balancefrom = 0, balanceto = 0;
    bool transactionDone = false;

    for (const QJsonValue& value : dataArray) {
        QJsonObject dataObject = value.toObject();
        QString dataAccountnumber = dataObject.value("Accountnumber").toString();

        if (dataAccountnumber == fromaccountnumber) {
            qDebug() << "Data found";   // Data found in the database
            balancefrom = dataObject.value("balance").toInt();
            qDebug() << balancefrom;
            if (transactionAmount > balancefrom) {
                response = "HTTP/1.0 200 OK\r\n\r\n";
                response += "False, Transaction can't be done: transaction amount is less than from account balance";
                transactionDone = true;
                break;
            } else {
                for (const QJsonValue& value : dataArray) {
                    QJsonObject dataObject = value.toObject();
                    QString dataAccountnumberto = dataObject.value("Accountnumber").toString();

                    if (dataAccountnumberto == toaccountnumber) {
                        balanceto = dataObject.value("balance").toInt();
                        qDebug() << balanceto;
                        dataObject["balance"] = balanceto + transactionAmount;
                        qDebug() << "to balance is now ::" << dataObject["balance"];
                        break;
                    }
                }
            }

            dataObject["balance"] = balancefrom - transactionAmount;
            qDebug() << dataObject.value("balance").toInt();
            response = "HTTP/1.0 200 OK\r\n\r\n";
            response += "true Transaction is done ";
            transactionDone = true;
            break;
        }
    }

    if (!transactionDone) {
        qDebug() << "Transaction data not found in the database";
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Transaction data not found in the database";
    }

    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
}

void MyServer::processgetRequestgetbankdatabase(QTcpSocket* socket)
{
    qDebug() << "processgetRequestgetbankdatabase";

    QByteArray response;

    if (!clientmap.contains(socket)) {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Logged in Before, Please Log in First";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    QString autho = clientmap.value(socket).authority;
    qDebug() << autho;

    if (autho != "admin") {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Authorized";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    // Specify the file path to read the JSON data from
    QString filePath = "DataBase.json";

    // Create a file object
    QFile file(filePath);

    // Open the file in read-only mode
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open file for reading.";
        return;
    }

    // Read the JSON data from the file
    QByteArray jsonData = file.readAll();

    // Close the file
    file.close();

    response = "HTTP/1.0 200 OK\r\n\r\n";
    response+=jsonData;
    //response += res.toUtf8();
    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
    return;


}

void MyServer::processPostRequestcreateuser(QTcpSocket* socket, const QByteArray& requestData,QJsonObject& database)
{
    qDebug() << "processPostRequestcreateuser";

    QByteArray response;

    if (!clientmap.contains(socket)) {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Logged in Before, Please Log in First";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    QString autho = clientmap.value(socket).authority;
    qDebug() << autho;

    if (autho != "admin") {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Authorized";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    // Extract the JSON data from the request
    QByteArray jsonBytes = requestData.mid(requestData.indexOf('{'));

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data";
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();
    jsonObject["authority"]="standard";
    jsonObject["balance"]=0;

    // Process the jsonObject as needed
    qDebug() << "Received POST request with data:";
    qDebug() << "username:" << jsonObject["username"].toString();
    qDebug() << "Password:" << jsonObject["password"].toString();
    qDebug() << "AccountNumber:" << jsonObject["Accountnumber"].toString();


    // Get the existing user array from the database
    QJsonArray userArray = database["users"].toArray();

    // Append the new user to the user array
    userArray.append(jsonObject);

    // Update the user array in the database
    database["users"] = userArray;

    // Create a JSON document from the updated database
    QJsonDocument jsonDocument(database);

    // Convert the JSON document to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    // Specify the file path to save the JSON data
    QString filePath = "DataBase.json";

    // Create a file object
    QFile file(filePath);

    // Open the file in write mode
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing.";
        return;
    }

    // Write the JSON data to the file
    file.write(jsonData);

    // Close the file
    file.close();

    response = "HTTP/1.0 200 OK\r\n\r\n";
    response += "User Created Successfully";
    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
    return;
}

void MyServer::processPostRequestupdateuser(QTcpSocket* socket, const QByteArray& requestData,QJsonObject& database)
{
    qDebug() << "processPostRequestupdateuser";

    QByteArray response;

    if (!clientmap.contains(socket)) {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Logged in Before, Please Log in First";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    QString autho = clientmap.value(socket).authority;
    qDebug() << autho;

    if (autho != "admin") {
        response = "HTTP/1.0 200 OK\r\n\r\n";
        response += "Not Authorized";
        socket->write(response);
        socket->flush();
        socket->waitForBytesWritten();
        return;
    }

    // Extract the JSON data from the request
    QByteArray jsonBytes = requestData.mid(requestData.indexOf('{'));

    // Parse the JSON data
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonBytes);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        qDebug() << "Invalid JSON data";
        return;
    }

    QJsonObject jsonObject = jsonDoc.object();

    // Process the jsonObject as needed
    qDebug() << "Received POST request with data:";
    qDebug() << "username:" << jsonObject["username"].toString();
    qDebug() << "Password:" << jsonObject["password"].toString();
    qDebug() << "AccountNumber:" << jsonObject["Accountnumber"].toString();


}