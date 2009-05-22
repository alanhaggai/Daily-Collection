#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

#include "DbConnect.h"

QSqlDatabase DbConnect::db;
QString DbConnect::database;
QString DbConnect::username;
QString DbConnect::password;

bool
DbConnect::Connect()
{
    if ( db.isOpen() )
        return true;

    database = "daily_collection";
    username = "root";
    password = "cubegin";

    // Set up connection related information via MySQL driver.
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName(database);
    db.setUserName(username);
    db.setPassword(password);

    // Check if connection to the database succeeded or not.
    if ( !db.open() )
    {
        QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Warning, "MySQL Database Connection Error",
                "Unable to connect to the database with the current settings.\
                \n\nPlease check if MySQL server is running or not, and, \
                start it if needed.\n\n", QMessageBox::Ok );
        msgbox->exec();

        return false;
    }
    else
        return true;
}

void
DbConnect::Disconnect()
{
    db.close();
}
