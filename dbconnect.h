#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

inline bool dbConnect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dailycollection.db");

    if ( !db.open() ) {
        qDebug() << db.lastError();
        qFatal("Failed to connect.");
        return false;
    }
    else {
        return true;
    }
}

#endif
