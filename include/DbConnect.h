/*!
 * \file  DbConnect.h
 * \brief Provides static functions/data members for connecting to database.
 */
#ifndef DBCONNECT_H
#define DBCONNECT_H

#include <QSqlDatabase>

class DbConnect {
    public:
        static bool Connect();
        static void Disconnect();
    private:
        static QSqlDatabase db;
};

#endif
