#include "server.h"
#include"DataBase.h"
#include"thread.h"

struct ClientData
{
    QString name;
    QString authority;
};

QByteArray Data_Base;
QJsonObject userTable;
QMap<QTcpSocket *,ClientData> clientmap;

/*MyServer::MyServer(QObject *parent) :
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
*/
MyServer::MyServer(QObject *parent)
{
    //connect(&server, &QTcpServer::newConnection, this, &Server::newConnection);
    Data_Base=Create_DataBase(userTable);


    if(!listen(QHostAddress::Any, 22))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void MyServer::incomingConnection(qintptr socketDescriptor) {

   // qDebug() << socketDescriptor << " Connecting...";
    qDebug()<<"new connection";
    Thread *worker = new Thread(socketDescriptor);
    QThread* thread = new QThread;

    worker->moveToThread(thread);

    QTcpSocket *clientSocket = new QTcpSocket(this);
    clientSocket->setSocketDescriptor(socketDescriptor);

    connect(clientSocket, &QTcpSocket::readyRead, this, &MyServer::ReadRequest);
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

    thread->start();
}
void MyServer::ReadRequest()
{

    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());

    qDebug() << socket ;

    QByteArray requestData = socket->readAll();
    qDebug() << "Client request: " << requestData;

    QString requeststring(requestData);

    if(requeststring.startsWith("GET"))
    {
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
        QByteArray response = "HTTP/1.0 200 OK\r\n\r\n";
        response+="Hello client this is a PUT request";
        socket->write(response);
    }
    else if(requeststring.startsWith("DELETE"))
    {
        if(requeststring.contains("/deleteuser"))
        {
            processDeleteRequestdeleteuser(socket,requestData,userTable);
        }
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
    if (!databaseFile.open(QIODevice::ReadWrite)) {
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

    for (int i = 0; i < dataArray.size(); ++i) {
        QJsonObject dataObject = dataArray[i].toObject();
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

            dataArray[i] = dataObject;

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

    // Update the users array in the databaseObject
    databaseObject["users"] = dataArray;

    // Write the modified JSON back to the file
    if (!databaseFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {  // Open the file in WriteOnly mode, truncating the existing content
        qDebug() << "Failed to open database file for writing";
        return;
    }

    QJsonDocument updatedDatabaseDoc(databaseObject);
    databaseFile.write(updatedDatabaseDoc.toJson());
    databaseFile.close();

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

    for (int i = 0; i < dataArray.size(); ++i) {
        QJsonObject dataObject = dataArray[i].toObject();
        //QJsonObject dataObject = value.toObject();
        QString dataAccountnumber = dataObject.value("Accountnumber").toString();
        if (dataAccountnumber == fromaccountnumber) {
            qDebug() << "Data found";   // Data found in the database
            balancefrom = dataObject.value("balance").toInt();
            qDebug() << balancefrom;
            if (transactionAmount > balancefrom) {
                response = "HTTP/1.0 200 OK\r\n\r\n";
                response += "False, Transaction can't be done: transaction amount is less than from account balance";
                //transactionDone = true;
                break;
            } else {
                for (int j = 0; j < dataArray.size(); ++j) {
                    //QJsonObject dataObject = value.toObject();
                    QJsonObject dataObject = dataArray[j].toObject();
                    QString dataAccountnumberto = dataObject.value("Accountnumber").toString();

                    if (dataAccountnumberto == toaccountnumber) {
                        balanceto = dataObject.value("balance").toInt();
                        qDebug() << balanceto;
                        dataObject["balance"] = balanceto + transactionAmount;
                        dataArray[j] = dataObject;

                        qDebug() << "to balance is now ::" << dataObject["balance"];
                        break;
                    }
                }
            }

            dataObject["balance"] = balancefrom - transactionAmount;
            dataArray[i] = dataObject;
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

    // Update the users array in the databaseObject
    databaseObject["users"] = dataArray;

    // Write the modified JSON back to the file
    if (!databaseFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {  // Open the file in WriteOnly mode, truncating the existing content
        qDebug() << "Failed to open database file for writing";
        return;
    }

    QJsonDocument updatedDatabaseDoc(databaseObject);
    databaseFile.write(updatedDatabaseDoc.toJson());
    databaseFile.close();

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


    QFile databaseFile("DataBase.json");
    if (!databaseFile.open(QIODevice::ReadWrite)) {
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
    QJsonArray userArray = databaseObject.value("users").toArray();



    // Get the existing user array from the database
   // QJsonArray userArray = database["users"].toArray();

    // Append the new user to the user array
    userArray.append(jsonObject);

    // Update the user array in the database
    databaseObject["users"] = userArray;

    // Create a JSON document from the updated database
    QJsonDocument jsonDocument(databaseObject);

    // Convert the JSON document to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    // Write the modified JSON back to the file
    if (!databaseFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {  // Open the file in WriteOnly mode, truncating the existing content
        qDebug() << "Failed to open database file for writing";
        return;
    }
    databaseFile.write(jsonData);
    databaseFile.close();

    response = "HTTP/1.0 200 OK\r\n\r\n";
    response += "User Created Successfully";
    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
    return;
}

void MyServer::processPostRequestupdateuser(QTcpSocket* socket, const QByteArray& requestData, QJsonObject& database)
{
    qDebug() << "processPostRequestupdateuser";

    if (!clientmap.contains(socket)) {
        sendResponse(socket, "Not Logged in Before, Please Log in First");
        return;
    }

    QString autho = clientmap.value(socket).authority;
    qDebug() << autho;

    if (autho != "admin") {
        sendResponse(socket, "Not Authorized");
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

    QString accountNumber = jsonObject["Accountnumber"].toString();
    QString username = jsonObject["username"].toString();
    QString password = jsonObject["password"].toString();

    QFile databaseFile("DataBase.json");
    if (!databaseFile.open(QIODevice::ReadWrite)) {
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

    bool updateDone = false;


    for (int i = 0; i < dataArray.size(); ++i) {
        QJsonObject dataObject = dataArray[i].toObject();
        QString dataAccountNum = dataObject.value("Accountnumber").toString();

        if (dataAccountNum == accountNumber && dataObject["authority"] == "standard") {
            qDebug() << "Data found";   // Data found in the database
            if (!username.isEmpty())
                dataObject["username"] = username;
            if (!password.isEmpty())
                dataObject["password"] = password;

            dataArray[i]=dataObject;

            // Update the users array in the databaseObject
            databaseObject["users"] = dataArray;

            // Write the modified JSON back to the file
            if (!databaseFile.open(QIODevice::WriteOnly | QIODevice::Truncate)) {  // Open the file in WriteOnly mode, truncating the existing content
                qDebug() << "Failed to open database file for writing";
                return;
            }

            QJsonDocument updatedDatabaseDoc(databaseObject);
            databaseFile.write(updatedDatabaseDoc.toJson());
            databaseFile.close();

            sendResponse(socket, "Update is done");
            qDebug() << "New update is " << dataObject["username"] << dataObject["password"];
            updateDone = true;
            break;
        }
    }
    if (!updateDone) {
        qDebug() << "Update cannot be done in the database";
        sendResponse(socket, "Update cannot be done in the database");
    }
}

void MyServer::sendResponse(QTcpSocket* socket, const QString& responseMessage)
{
    QByteArray response = "HTTP/1.0 200 OK\r\n\r\n" + responseMessage.toUtf8();
    socket->write(response);
    socket->flush();
    socket->waitForBytesWritten();
}

void MyServer::processDeleteRequestdeleteuser(QTcpSocket* socket, const QByteArray& requestData,QJsonObject& database)
{
    qDebug() << "processDeleteRequestdeleteuser";

    if (!clientmap.contains(socket)) {
        sendResponse(socket, "Not Logged in Before, Please Log in First");
        return;
    }

    QString autho = clientmap.value(socket).authority;
    qDebug() << autho;

    if (autho != "admin") {
        sendResponse(socket, "Not Authorized");
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
    qDebug() << "Received DELETE request with data:";
    qDebug() << "AccountNumber:" << jsonObject["Accountnumber"].toString();

    QString accountNumber = jsonObject["Accountnumber"].toString();

    QFile databaseFile("DataBase.json");
    if (!databaseFile.open(QIODevice::ReadWrite)) {
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

    bool DeleteDone = false;

    // Find and remove the user with the specified account number
    for (int i = 0; i < dataArray.size(); ++i) {
        QJsonObject userObject = dataArray[i].toObject();
        if (userObject["Accountnumber"].toString() == accountNumber && userObject["authority"] == "standard") {
            qDebug() << "Data found";
            dataArray.removeAt(i);
            // Update the user array in the database object
            databaseObject["users"] = dataArray;

            // Convert the updated database object back to JSON
            QJsonDocument updatedJsonDocument(databaseObject);
            QByteArray updatedJsonData = updatedJsonDocument.toJson();

            // Write the updated JSON data back to the file
            if (!databaseFile.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
                qDebug() << "Failed to open file for writing.";
                return;
            }

            databaseFile.write(updatedJsonData);
            databaseFile.close();
            sendResponse(socket, "Delete is done");
            DeleteDone = true;
            break;
        }
    }


    if (!DeleteDone) {
        qDebug() << "Delete cannot be done in the database";
        sendResponse(socket, "Delete cannot be done in the database");
    }

}
