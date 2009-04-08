/*!
 * \file  DbConnect.h
 * \brief Provides function for connecting to database.
 */
#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

#define HOSTNAME     "localhost"         //!< Hostname of the database server
#define DATABASENAME "daily_collection"  //!< Database name
#define USERNAME     "root"              //!< Username for accessing the database
#define PASSWORD     "cubegin"           //!< Password

/*!
 * \brief Tries to connect to the database and returns the status.
 * \return Boolean flag - \a true on success and \a false on failure to connect to the
 * database.
 */
inline bool
DbConnect() {
    // Set up connection related information via MySQL driver.
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(HOSTNAME);
    db.setDatabaseName(DATABASENAME);
    db.setUserName(USERNAME);
    db.setPassword(PASSWORD);

    // Check if connection to the database succeeded or not.
    if ( !db.open() ) {
        // If connection to database failed, spit out the error message and
        // return false.
        qDebug() << db.lastError();
        qFatal("Failed to connect.");
        return false;
    }
    else {
        // Return true on successful connection to the database.
        return true;
    }
}

#endif
