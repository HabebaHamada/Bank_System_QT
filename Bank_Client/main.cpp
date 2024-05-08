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


   client.connectToHost("192.168.1.7",22);

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
  // client.Update_User("A01","username:hanen");
   client.Delete_User("A01");
   client.View_Bank_DataBase();

  /*  bool IsLoggin = false;
    qInfo() << "Welcome to our Banking System..." ;
    qInfo() << "Please choose Who are you? " ;
    qInfo() << "\"User\" or \"Admin\" or \"Exit\" " ;
    string cselection;
    cin >> cselection;
    QString qselection = QString::fromStdString(cselection);
    if(qselection.toUpper() == "USER")
    {
        //qInfo() << "AsmaaMohamed";
        UserManage user;
        user.connectToHost("127.0.0.1" , 1234);
        while(!IsLoggin)
        {
            IsLoggin = user.login();
            user.cleanScreen();
            if(!IsLoggin)
            {
                qInfo() << "Username or Password is uncorrect, Please try again..." ;
            }
        }
        while(IsLoggin)
        {
            user.start(IsLoggin);
        }
    }
    else if(qselection.toUpper() == "ADMIN")
    {
        AdminManager admin;
        admin.connectToHost("127.0.0.1" , 1234);
        while(!IsLoggin)
        {
            IsLoggin = admin.login();
            admin.cleanScreen();
            if(!IsLoggin)
            {
                qInfo() << "Username or Password is uncorrect, Please try again..." ;
            }
        }
        while(IsLoggin)
        {
            admin.start(IsLoggin);
        }
    }
    else if (qselection.toUpper() == "EXIT")
    {
        qInfo() << "Banking System is Exiting now...";
        QCoreApplication::exit(0);
    }
    else
    {
        qInfo() << "Invalid Username...";
    }
    QCoreApplication::exit(0);*/


    return a.exec();
}
