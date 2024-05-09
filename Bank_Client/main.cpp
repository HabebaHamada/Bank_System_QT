#include <QCoreApplication>
#include <QDebug>
#include <QTextStream>
#include <iostream>

#include"client.h"
#include"Bank_System_Requests.h"

using namespace std ;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Client client;
    client.connectToHost("192.168.1.7",22);

    //  client.Transfer_Amount("A04","A01",300);
    /* client.View_Account_Balance("A00");
    client.Get_Account_Number();
    client.Get_Account_Number("Mohammed");*/
    // client.Make_Transaction("A00",-500);
    // client.Get_Account_Number("aya");
    //client_1.Log_in("mohammed" ,"30");

    //client.Create_User("username:hanen,password:56789,accountnumber:A30");
    //client.View_Bank_DataBase();
    // client.Update_User("A01","username:hanen");
    //  client.Delete_User("A01");
    // client.View_Bank_DataBase();
    // Assuming the Client class is defined and implemented elsewhere

    bool IsLoggin = false;
    QString userInput;

    QTextStream input(stdin);
    QTextStream output(stdout);

    qDebug() << "Welcome to our Banking System...";
    qDebug() << "type ‘help’ to print the manual";

    do
    {
        userInput = input.readLine().trimmed(); // Read user input and remove leading/trailing whitespace
        QString role;
        if (userInput.contains("login", Qt::CaseInsensitive))
        {
            qDebug() << "Please choose Who are you?";
            qDebug() << "\"User\" or \"Admin\"";
            QString usertype= input.readLine().trimmed();
            role=usertype;
            if (IsLoggin)
            {
                qInfo() << "Already logged in...\n";
                break;
            }
            qInfo() << "Enter User name:";
            QString username = input.readLine().trimmed();

            qInfo() << "Enter Password:";
            QString password = input.readLine().trimmed();

            // Perform login logic here
            if(usertype.contains("user", Qt::CaseInsensitive))
            {
             client.Log_in(username, password, "standard");
            }
            else if(usertype.contains("admin", Qt::CaseInsensitive))
            {
                client.Log_in(username, password, "admin");
            }
            IsLoggin = true;
        }
        else if(userInput.contains("Accountnumber", Qt::CaseInsensitive))
        {
            qInfo() << role;
            if(role.contains("user", Qt::CaseInsensitive))
            {
                client.Get_Account_Number();
            }
            else if(role.contains("admin", Qt::CaseInsensitive))
            {
                qInfo() << "Enter User name you want its Account Number:";
                QString name = input.readLine().trimmed();
                client.Get_Account_Number(name);
            }
        }
        else if (userInput.contains("help", Qt::CaseInsensitive))
        {
            qInfo() << "The following is the list of all supported commands:\n"
                      " login <username> <pass> -> prints account number\n"
                      " view Account balance <accountnumber> -> prints balance of given account\n"
                      " Accountnumber <username>  -> prints account number of given user\n"
                      " Make Transaction <accountnumber, transaction amount> -> make money transaction\n"
                      " Transfer Amount <fromaccountnumber, toaccountnumber, transferamount> -> make money transfer\n";

            qInfo() << "The following is the list of all special supported commands for admin only:\n"
                       " View Bank Database -> prints Bank Database\n"
                       " Create New User<userdate> \n"
                       " Update New User<userdate> \n";
                       " Delete New User<userdate> \n";
        }
        else if (userInput.contains("exit", Qt::CaseInsensitive))
        {
            break; // Exit the loop and continue with program termination
        }
        else
        {
            qInfo() << "Invalid Command. Type ‘help’ to print the manual\n";
        }
    } while (true);

    qInfo() << "Banking System is Exiting now...";
    return a.exec();
}
