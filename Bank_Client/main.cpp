#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <iostream>

#include "client.h"

using namespace std;

void printHelp() {
    qInfo() << "The following is the list of all supported commands:\n"
               " login <username> <pass> -> prints account number\n"
               " view Account balance <accountnumber> -> prints balance of given account\n"
               " Accountnumber <username>  -> prints account number of given user\n"
               " Make Transaction <accountnumber, transaction amount> -> make money transaction\n"
               " Transfer Amount <fromaccountnumber, toaccountnumber, transferamount> -> make money transfer\n";

    qInfo() << "The following is the list of all special supported commands for admin only:\n"
               " View Bank Database -> prints Bank Database\n"
               " Create New User<userdate> \n"
               " Update New User<userdate> \n"
               " Delete New User<userdate> \n";
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;

  //  client.Log_in("Ahmed","30","admin");

    //client.Update_User("A02","username:Mona");
 //   client.View_Bank_DataBase();
   // client.View_Account_Balance("A02");

     bool isLoggedIn = false;
    QString role;
    QTextStream input(stdin);
    QTextStream output(stdout);

    qDebug() << "Welcome to our Banking System...";
    qDebug() << "Type 'help' to print the manual";

    while (true) {
        QString userInput = input.readLine().trimmed(); // Read user input and remove leading/trailing whitespace

        if (userInput.contains("login", Qt::CaseInsensitive)) {
            if (isLoggedIn) {
                qInfo() << "Already logged in...\n";
                break;
            }

            qDebug() << "Please choose who you are?";
            qDebug() << "\"User\" or \"Admin\"";
            QString userType = input.readLine().trimmed();
            role = userType;

            qInfo() << "Enter User name:";
            QString username = input.readLine().trimmed();

            qInfo() << "Enter Password:";
            QString password = input.readLine().trimmed();

            // Perform login logic here
            if (userType.contains("user", Qt::CaseInsensitive)) {
                client.Log_in(username, password, "standard");
            } else if (userType.contains("admin", Qt::CaseInsensitive)) {
                client.Log_in(username, password, "admin");
            }
            isLoggedIn = true;
        } else if (userInput.contains("Accountnumber", Qt::CaseInsensitive)) {
            if (role.contains("user", Qt::CaseInsensitive)) {
                client.Get_Account_Number();
            } else if (role.contains("admin", Qt::CaseInsensitive)) {
                qInfo() << "Enter User name you want its Account Number:";
                QString name = input.readLine().trimmed();
                client.Get_Account_Number(name);
            }
        } else if (userInput.contains("View Bank Database", Qt::CaseInsensitive)) {
                client.View_Bank_DataBase();
        } else if (userInput.contains("help", Qt::CaseInsensitive)) {
            printHelp();
        } else if (userInput.contains("exit", Qt::CaseInsensitive)) {
            break; // Exit the loop and continue with program termination
        } else {
            qInfo() << "Invalid Command. Type 'help' to print the manual\n";
        }
    }

    qInfo() << "Banking System is Exiting now...";


    return a.exec();
}
