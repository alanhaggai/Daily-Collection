#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>

#include "DbConnect.h"

QSqlDatabase DbConnect::db;

bool
DbConnect::Connect() {
    if ( db.isOpen() )
        return true;

    // Set up connection related information via MySQL driver.
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("daily_collection.db");

    // Check if connection to the database succeeded or not.
    if ( !db.open() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Warning, "Database connection error",
                "Error occurred while trying to connect to the database.",
                QMessageBox::Ok );
            msgbox->exec();

            return false;
        }
    else
        return true;
}

void
DbConnect::Disconnect() {
    db.close();
}
