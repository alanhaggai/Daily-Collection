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

enum {
    DATE,
    PAID,
};

//! Constructor
DebtorTransactionsDialog::DebtorTransactionsDialog(QDialog* parent) :
        QDialog( parent, Qt::Tool ) {

    setupUi(this);  // Setup UI

    connect( serial_edit, SIGNAL( textChanged(const QString&) ), this,
            SLOT( SerialEditTextChanged(const QString&) ) );
    connect( clear_button, SIGNAL( clicked() ), this, SLOT( Clear() ) );
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

    if ( debtor_serial.length() < 4 ) {
            return;
        }

    QSqlQuery query;
    query.prepare("SELECT debtor.name, debtor.amount, SUM(transaction.paid)\
            FROM debtor, transaction WHERE debtor.serial = :debtor_serial\
            AND debtor.id = transaction.debtor_id ORDER BY transaction.date");
    query.bindValue( ":debtor_serial", debtor_serial );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    query.next();

    qint16 row = 0;
    QString debtor_name;
    qint16 debtor_amount;
    qint16 debtor_paid;
    qint16 debtor_balance;

    debtor_name    = query.value(0).toString();
    debtor_amount  = query.value(1).toInt();
    debtor_paid    = query.value(2).toInt();
    debtor_balance = debtor_amount - debtor_paid;

    name_edit->setText(debtor_name);
    amount_edit->setText( QString::number(debtor_amount) );
    paid_edit->setText( QString::number(debtor_paid) );
    balance_edit->setText( QString::number(debtor_balance) );

    query.prepare("SELECT transaction.date, transaction.paid FROM debtor,\
            transaction WHERE debtor.serial = :debtor_serial AND debtor.id\
            = transaction.debtor_id ORDER BY transaction.date");
    query.bindValue( ":debtor_serial", debtor_serial );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    installments_edit->setText( QString::number( query.size() ) );
    table_widget->setRowCount( query.size() );

    while ( query.next() ) {
            QString date;
            QString paid;

            date = query.value(DATE).toString();
            paid = query.value(PAID).toString();

            QTableWidgetItem* date_item = new QTableWidgetItem;
            QTableWidgetItem* paid_item = new QTableWidgetItem;

            date_item->setText(date);
            paid_item->setText(paid);

            table_widget->setItem( row,   DATE, date_item );
            table_widget->setItem( row++, PAID, paid_item );
        }
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

    serial_edit->setFocus();
}
