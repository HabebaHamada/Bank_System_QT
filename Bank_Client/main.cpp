<<<<<<< HEAD
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
=======
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
        } else if (userInput.contains("Accountbalance", Qt::CaseInsensitive)) {
                qInfo() << "Enter Account number you want its Account balance:";
                QString Accountnumber = input.readLine().trimmed();
                client.View_Account_Balance(Accountnumber);

        } else if (userInput.contains("View Bank Database", Qt::CaseInsensitive)) {
                client.View_Bank_DataBase();

        } else if (userInput.contains("Make Transaction", Qt::CaseInsensitive)) {
            qInfo() << "Enter The Account Number:";
            QString Account = input.readLine().trimmed();
            qInfo() << "Enter The money amount:";
            int32_t transactionMoney = input.readLine().trimmed().toInt();
            client.Make_Transaction(Account,transactionMoney);

        } else if (userInput.contains("Transfer Amount", Qt::CaseInsensitive)) {
            qInfo() << "Enter The Account Number you want to transfer from:";
            QString Accountfrom = input.readLine().trimmed();
            qInfo() << "Enter The Account Number you want to transfer to:";
            QString Accountto = input.readLine().trimmed();
            qInfo() << "Enter The money amount:";
            int32_t transactionMoney = input.readLine().trimmed().toInt();
            client.Transfer_Amount(Accountfrom,Accountto,transactionMoney);

        } else if (userInput.contains("Deleteuser", Qt::CaseInsensitive)) {
            qInfo() << "Enter Account number you want to delete:";
            QString Accountnumber = input.readLine().trimmed();
            client.Delete_User(Accountnumber);

        } else if (userInput.contains("Createuser", Qt::CaseInsensitive)) {
            qInfo() << "Enter new User data as a form of \n username:yourname,password:yourpassword,accountnumber:youraccountnumber";
            QString data = input.readLine().trimmed();
            client.Create_User(data);

        } else if (userInput.contains("Updateuser", Qt::CaseInsensitive)) {

            qInfo() << "Enter Account number you want to Update:";
            QString Accountnumber = input.readLine().trimmed();
            qInfo()<< "which data you want to update : 1.name \n2.Password\n3.both";
            QString datatype=input.readLine().trimmed();
            if(datatype.contains("name", Qt::CaseInsensitive))
            {
                qInfo() << "Enter new User name as a form of \n username:yournewname";
                QString data = input.readLine().trimmed();
                client.Update_User(Accountnumber,data);
            }
            else if(datatype.contains("password", Qt::CaseInsensitive))
            {
                qInfo() << "Enter new password as a form of \n password:yourpassword";
                QString data = input.readLine().trimmed();
                client.Update_User(Accountnumber,data);
            }
            else if(datatype.contains("both", Qt::CaseInsensitive))
            {
                qInfo() << "Enter new Data as a form of \n username:yournewname,password:yourpassword";
                QString data = input.readLine().trimmed();
                client.Update_User(Accountnumber,data);
            }

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
>>>>>>> a55d8750151f0ad51a86ef31b98a3035fe87fe49
