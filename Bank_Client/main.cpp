#include <QCoreApplication>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QDebug>

#include"client.h"
#include"Bank_System_Requests.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;
  //  Client client_1;


    client.connectToHost("192.168.1.7",22);
   // client_1.connectToHost("192.168.1.8",22);

  /*  client.get("http://192.168.1.8:22/gett");
    client.get("http://192.168.1.8:22/gett222");

    client.post("http://192.168.1.8:22/post","this is a post data from client");
    client.put("http://192.168.1.8:22/put","this is a put data from client");


    client_1.get("http://192.168.1.8:22/gettfromclient_1");

    client_1.post("http://192.168.1.8:22/post","this is a post data from client_1");
    client_1.put("http://192.168.1.8:22/put","this is a put data from client_1");*/


    client.Log_in("Ahmed" ,"30","admin");
  //  client.Transfer_Amount("A04","A01",300);
   /* client.View_Account_Balance("A00");
    client.Get_Account_Number();
    client.Get_Account_Number("Mohammed");*/
  // client.Make_Transaction("A00",-500);
 // client.Get_Account_Number("aya");
    //client_1.Log_in("mohammed" ,"30");

   //client.Create_User("username:hanen,password:56789,accountnumber:A30");
   //client.View_Bank_DataBase();
    client.Update_User("A00","username:hanen");

   /* // Create a JSON object for the user table
    QJsonObject userTable;

    // Create a JSON array to hold user entries
    QJsonArray plot_array;

    // User 1
    QJsonObject user1;
    user1["username"] = "user1";
    user1["password"] = "password1";
    user1["authority"] = "standard";
    plot_array.append(user1);


    // User 2
    QJsonObject user2;
    user2["username"] = "user2";
    user2["password"] = "password2";
    user2["authority"] = "admin";
    plot_array.append(user2);


    // Add the users array to the user table object
    userTable["users"] = plot_array;

    // Create a JSON document from the user table
    QJsonDocument jsonDocument(userTable);

    // Convert the JSON document to a string
    QString jsonString = jsonDocument.toJson();

    // Convert the JSON document to a QByteArray
    QByteArray jsonData = jsonDocument.toJson();

    // Print the JSON string
    qDebug() << jsonString;

    // Specify the file path to save the JSON data
    QString filePath = "DataBase.json";

    // Create a file object
    QFile file(filePath);

    // Open the file in write mode
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing.";
        return 1;
    }

    // Write the JSON data to the file
    file.write(jsonData);

    // Close the file
    file.close();

    qDebug() << "JSON data saved to file: " << filePath;*/

    return a.exec();
}
