/*!
 * \file  DebtorTransactionsDialog.cpp
 * \brief Lists transactions made by a debtor
 *
 * \ingroup Debtor
 */

#include "DebtorTransactionsDialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>

enum {
    DATE,
    PAID
};

//! Constructor
DebtorTransactionsDialog::DebtorTransactionsDialog(QDialog* parent) :
        QDialog(parent) {

    setupUi(this);  // Setup UI

    connect( serial_edit, SIGNAL( textChanged(const QString&) ), this,
            SLOT( SerialEditTextChanged(const QString&) ) );
    connect( clear_button,  SIGNAL( clicked() ), this, SLOT( Clear() ) );
}

/*
 * This function executes when serial number is entered/changed. It fetches the
 * debtor's name, which corresponds to the serial number, and selects the
 * proper installment.
 */
void
DebtorTransactionsDialog::SerialEditTextChanged(const QString& debtor_serial) {
    table_widget->clearContents();
    table_widget->setRowCount(0);

    if ( debtor_serial.length() < 4 )
        return;

    QSqlQuery query;
    query.prepare("SELECT debtor.name, debtor.amount, SUM(transactions.paid)\
            FROM debtor, transactions WHERE debtor.serial = :debtor_serial\
            AND debtor.id = transactions.debtor_id ORDER BY transactions.date");
    query.bindValue( ":debtor_serial", debtor_serial );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query execution failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    query.next();

    QString debtor_name;
    qint16 debtor_amount;
    qint16 debtor_paid;
    qint16 debtor_balance;

    debtor_name    = query.value(0).toString();
    debtor_amount  = query.value(1).toInt();
    debtor_paid    = query.value(2).toInt();
    debtor_balance = debtor_amount - debtor_paid;

    if ( debtor_paid == 0 ) {
            QSqlQuery query_name;
            query_name.prepare("SELECT name FROM debtor WHERE serial = :debtor_serial");
            query_name.bindValue( ":debtor_serial", debtor_serial );

            if ( !query_name.exec() ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Critical, "Query execution failed",
                        "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                        QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }

            query_name.next();

            debtor_name = query_name.value(0).toString();
        }


    name_edit->setText(debtor_name);
    amount_edit->setText( QString::number(debtor_amount) );
    paid_edit->setText( QString::number(debtor_paid) );
    balance_edit->setText( QString::number(debtor_balance) );

    query.prepare("SELECT transactions.date, transactions.paid FROM debtor,\
            transactions WHERE debtor.serial = :debtor_serial AND debtor.id\
            = transactions.debtor_id ORDER BY transactions.date");
    query.bindValue( ":debtor_serial", debtor_serial );

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
            QString date;
            QString paid;

            date = query.value(DATE).toDate().toString("dd-MM-yyyy");
            paid = query.value(PAID).toString();

            QTableWidgetItem* date_item = new QTableWidgetItem;
            QTableWidgetItem* paid_item = new QTableWidgetItem;

            date_item->setText(date);
            paid_item->setText(paid);

            table_widget->setItem( row,   DATE, date_item );
            table_widget->setItem( row++, PAID, paid_item );
        }
    while ( query.next() );

    installments_edit->setText( QString::number(row) );
}

void
DebtorTransactionsDialog::Clear() {
    serial_edit->setText("");
    name_edit->setText("");
    amount_edit->setText("");
    paid_edit->setText("");
    balance_edit->setText("");
    installments_edit->setText("");
    table_widget->clearContents();
    table_widget->setRowCount(0);

    serial_edit->setFocus();
}
