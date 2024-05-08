#include"DataBase.h"


QByteArray Create_DataBase(QJsonObject& userTable)
{

    // Create a JSON object for the user table

    // Create a JSON array to hold user entries
    QJsonArray plot_array;

    // User 1
    QJsonObject user1;
    user1["username"] = "Ahmed";
    user1["fullname"] = "Ahmed zaed";
    user1["Age"] = "20";
    user1["password"] = "30";
    user1["authority"] = "admin";
    user1["Accountnumber"] = "A00";
    user1["balance"] = 1000;
    plot_array.append(user1);


    // User 2
    QJsonObject user2;
    user2["username"] = "Mohammed";
    user2["fullname"] = "Mohammed hassan";
    user2["Age"] = "24";
    user2["password"] = "15109";
    user2["authority"] = "standard";
    user2["Accountnumber"] = "A01";
    user2["balance"] = 1560;

    plot_array.append(user2);


    // User 3
    QJsonObject user3;
    user3["username"] = "Habeba";
    user3["fullname"] = "Habeba Hamada";
    user3["Age"] = "23";
    user3["password"] = "822000";
    user3["authority"] = "standard";
    user3["Accountnumber"] = "A02";
    user3["balance"] = 2000;

    plot_array.append(user3);


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
    }

    // Write the JSON data to the file
    file.write(jsonData);

    // Close the file
    file.close();

    qDebug() << "JSON data saved to file: " << filePath;

   return jsonDocument.toJson();

}
