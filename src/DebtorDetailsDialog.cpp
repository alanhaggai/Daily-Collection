/*!
 * \file  DebtorDetailsDialog.cpp
 * \brief Displays daybook of each details
 *
 * \ingroup Debtor
 */

#include "DebtorDetailsDialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

//! Enumerator constants for representing Table Widget columns
enum {
    SERIAL,
    NAME,
    AGENT,
    ADDRESS,
    AMOUNT,
    PAID,
    BALANCE,
    PHONE,
    DATE
};

/*!
 * Constructor that creates a window for displaying details of debtors.
 * UI is setup. Table Widget column widths are set.
 */
DebtorDetailsDialog::DebtorDetailsDialog(QDialog* parent) :
        QDialog(parent) {
    setupUi(this);

    // Set column widths for Table Widget
    table_widget->setColumnWidth( SERIAL,  50);
    table_widget->setColumnWidth( NAME,    200);
    table_widget->setColumnWidth( AGENT,   200);
    table_widget->setColumnWidth( ADDRESS, 200);
    table_widget->setColumnWidth( AMOUNT,  70);
    table_widget->setColumnWidth( PAID,    70);
    table_widget->setColumnWidth( BALANCE, 70);
    table_widget->setColumnWidth( PHONE,   100);
    table_widget->setColumnWidth( DATE,    100);

    connect( serial_edit, SIGNAL( textChanged(QString) ), this,
            SLOT( SerialEditTextChanged(QString) ) );
    connect( name_edit, SIGNAL( textChanged(QString) ), this,
            SLOT( NameEditTextChanged(QString) ) );
}

/*!
 * Populates Table Widget with details of debtors that match more than three
 * digits of their serials.
 */
void
DebtorDetailsDialog::SerialEditTextChanged(QString debtor_serial) {
    table_widget->clearContents();
    table_widget->setRowCount(0);

    // Why should we waste time/resource on populating the Table Widget with an
    // unwanted humongous list?
    if ( debtor_serial.length() < 3 )
        return;

    QSqlQuery query;
    query.prepare("SELECT debtor.serial, debtor.name, agent.name,\
            debtor.address, debtor.amount, debtor.phone, debtor.date,\
            debtor.agent_id, debtor.id FROM debtor, agent WHERE\
            debtor.agent_id = agent.id AND debtor.serial LIKE :debtor_serial\
            ORDER BY serial ASC");
    query.bindValue( ":debtor_serial", "%" + debtor_serial + "%" );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query execution failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    qint32 count = 0;

    while ( query.next() )
        count++;

    table_widget->setRowCount(count);
    query.first();

    qint32 row = 0;

    do {
            QString debtor_serial;
            QString debtor_name;
            QString debtor_agent_name;
            QString debtor_address;
            qint16  debtor_amount;
            qint16  debtor_paid;
            qint16  debtor_balance;
            QString debtor_phone;
            QString debtor_date;
            QString agent_id;
            QString debtor_id;

            debtor_serial     = query.value(0).toString();
            debtor_name       = query.value(1).toString();
            debtor_agent_name = query.value(2).toString();
            debtor_address    = query.value(3).toString();
            debtor_amount     = query.value(4).toInt();
            debtor_phone      = query.value(5).toString();
            debtor_date       = query.value(6).toDate().toString("dd-MM-yyyy");
            agent_id          = query.value(7).toString();
            debtor_id         = query.value(8).toString();

            QTableWidgetItem* serial_item  = new QTableWidgetItem;
            QTableWidgetItem* name_item    = new QTableWidgetItem;
            QTableWidgetItem* agent_item   = new QTableWidgetItem;
            QTableWidgetItem* address_item = new QTableWidgetItem;
            QTableWidgetItem* amount_item  = new QTableWidgetItem;
            QTableWidgetItem* paid_item    = new QTableWidgetItem;
            QTableWidgetItem* balance_item = new QTableWidgetItem;
            QTableWidgetItem* phone_item   = new QTableWidgetItem;
            QTableWidgetItem* date_item    = new QTableWidgetItem;

            QSqlQuery query_sum;
            query_sum.prepare("SELECT SUM(transactions.paid) FROM transactions\
                    WHERE debtor_id = :debtor_id");
            query_sum.bindValue( ":debtor_id", debtor_id );

            if ( !query_sum.exec() ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Critical, "Query execution failed",
                        "Execution of query <b>" + query_sum.lastQuery()
                        + "</b>, failed.", QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }

            query_sum.next();

            debtor_paid    = query_sum.value(0).toInt();
            debtor_balance = debtor_amount - debtor_paid;

            serial_item->setText(debtor_serial);
            name_item->setText(debtor_name);
            agent_item->setText(debtor_agent_name);
            address_item->setText(debtor_address);
            amount_item->setText( QString::number(debtor_amount) );
            paid_item->setText( QString::number(debtor_paid) );
            balance_item->setText( QString::number(debtor_balance) );
            phone_item->setText(debtor_phone);
            date_item->setText(debtor_date);

            table_widget->setItem( row,   SERIAL,  serial_item );
            table_widget->setItem( row,   NAME,    name_item );
            table_widget->setItem( row,   AGENT,   agent_item );
            table_widget->setItem( row,   ADDRESS, address_item );
            table_widget->setItem( row,   AMOUNT,  amount_item );
            table_widget->setItem( row,   PAID,    paid_item );
            table_widget->setItem( row,   BALANCE, balance_item );
            table_widget->setItem( row,   PHONE,   phone_item );
            table_widget->setItem( row++, DATE,    date_item );
        }
    while ( query.next() );
}

/*!
 * Populates Table Widget with details of debtors that match more than three
 * digits of their serials.
 */
void
DebtorDetailsDialog::NameEditTextChanged(QString debtor_name) {
    table_widget->clearContents();
    table_widget->setRowCount(0);

    // Why should we waste time/resource on populating the Table Widget with an
    // unwanted humongous list?
    if ( debtor_name.length() < 3 )
        return;

    QSqlQuery query;
    query.prepare("SELECT debtor.serial, debtor.name, agent.name,\
            debtor.address, debtor.amount, debtor.phone, debtor.date,\
            debtor.agent_id, debtor.id FROM debtor, agent WHERE debtor.agent_id\
            = agent.id AND debtor.name LIKE :debtor_name ORDER BY serial ASC");
    query.bindValue( ":debtor_name", "%" + debtor_name + "%" );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query execution failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    qint32 count = 0;

    while ( query.next() )
        count++;

    table_widget->setRowCount(count);
    query.first();

    qint32 row = 0;

    do {
            QString debtor_serial;
            QString debtor_name;
            QString debtor_agent_name;
            QString debtor_address;
            qint16  debtor_amount;
            qint16  debtor_paid;
            qint16  debtor_balance;
            QString debtor_phone;
            QString debtor_date;
            QString agent_id;
            QString debtor_id;

            debtor_serial     = query.value(0).toString();
            debtor_name       = query.value(1).toString();
            debtor_agent_name = query.value(2).toString();
            debtor_address    = query.value(3).toString();
            debtor_amount     = query.value(4).toInt();
            debtor_phone      = query.value(5).toString();
            debtor_date       = query.value(6).toDate().toString("dd-MM-yyyy");
            agent_id          = query.value(7).toString();
            debtor_id         = query.value(8).toString();

            QTableWidgetItem* serial_item  = new QTableWidgetItem;
            QTableWidgetItem* name_item    = new QTableWidgetItem;
            QTableWidgetItem* agent_item   = new QTableWidgetItem;
            QTableWidgetItem* address_item = new QTableWidgetItem;
            QTableWidgetItem* amount_item  = new QTableWidgetItem;
            QTableWidgetItem* paid_item    = new QTableWidgetItem;
            QTableWidgetItem* balance_item = new QTableWidgetItem;
            QTableWidgetItem* phone_item   = new QTableWidgetItem;
            QTableWidgetItem* date_item    = new QTableWidgetItem;

            QSqlQuery query_sum;
            query_sum.prepare("SELECT SUM(transactions.paid) FROM transactions\
                    WHERE debtor_id = :debtor_id");
            query_sum.bindValue( ":debtor_id", debtor_id );

            if ( !query_sum.exec() ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Critical, "Query execution failed",
                        "Execution of query <b>" + query_sum.lastQuery()
                        + "</b>, failed.", QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }

            query_sum.next();

            debtor_paid    = query_sum.value(0).toInt();
            debtor_balance = debtor_amount - debtor_paid;

            serial_item->setText(debtor_serial);
            name_item->setText(debtor_name);
            agent_item->setText(debtor_agent_name);
            address_item->setText(debtor_address);
            amount_item->setText( QString::number(debtor_amount) );
            paid_item->setText( QString::number(debtor_paid) );
            balance_item->setText( QString::number(debtor_balance) );
            phone_item->setText(debtor_phone);
            date_item->setText(debtor_date);

            table_widget->setItem( row,   SERIAL,  serial_item );
            table_widget->setItem( row,   NAME,    name_item );
            table_widget->setItem( row,   AGENT,   agent_item );
            table_widget->setItem( row,   ADDRESS, address_item );
            table_widget->setItem( row,   AMOUNT,  amount_item );
            table_widget->setItem( row,   PAID,    paid_item );
            table_widget->setItem( row,   BALANCE, balance_item );
            table_widget->setItem( row,   PHONE,   phone_item );
            table_widget->setItem( row++, DATE,    date_item );
        }
    while ( query.next() );
}
