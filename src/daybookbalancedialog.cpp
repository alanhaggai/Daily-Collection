#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QProgressDialog>

#include "daybookbalancedialog.h"

const int SERIAL  = 0;
const int NAME    = 1;
const int AMOUNT  = 2;
const int BALANCE = 2;
const int PAID    = 3;

DaybookBalanceDialog::DaybookBalanceDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
    setupUi(this);

    dateCalendar->setGridVisible(true);

    QSqlQuery query;
    query.prepare("SELECT * FROM agent");
    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }
    while ( query.next() ) {
         agentCombo->addItem( query.value(1).toString() );
         agentMap[query.value(0).toInt()] = query.value(1).toString();
    }
    agentCombo->setCurrentIndex(-1);

    tableWidget->setColumnWidth( SERIAL,  50);
    tableWidget->setColumnWidth( NAME,    200);
    tableWidget->setColumnWidth( BALANCE, 100);

    connect( listButton, SIGNAL(clicked()), this, SLOT(populateTableWidget()) );
}

void DaybookBalanceDialog::populateTableWidget() {
    if ( -1 == agentCombo->currentIndex() ) {
        QMessageBox *msgbox = new QMessageBox(
            QMessageBox::Information, "Incomplete Fields",
            "All fields are to be filled.", QMessageBox::Ok );
        msgbox->show();

        return;
    }

    QSqlQuery query;
    query.prepare("SELECT debtor.serial, debtor.name, debtor.amount, SUM(transaction.paid) FROM debtor, transaction WHERE debtor.agent_id = :agentId AND debtor.id = transaction.debtor_id AND transaction.date <= :transactionDate GROUP BY debtor.serial ORDER BY debtor.serial");
    query.bindValue( ":agentId",         agentMap.key(agentCombo->currentText()) );
    query.bindValue( ":transactionDate", dateCalendar->selectedDate().toString(Qt::ISODate) );

    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    QProgressDialog progressDialog( "Retrieving data...", "Abort Fetch", 0, query.size(), this );
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setMinimumDuration(0);

    int row = 0, counter = 0;

    tableWidget->clearContents();

    while ( query.next() ) {
        progressDialog.setValue(counter++);
        qApp->processEvents();

        if ( progressDialog.wasCanceled() )
            break;

        QString debtorSerial  = query.value(SERIAL).toString();
        QString debtorName    = query.value(NAME).toString();
        QString debtorBalance = QString::number( query.value(AMOUNT).toInt() - query.value(PAID).toInt() );

        QTableWidgetItem *serialItem  = new QTableWidgetItem;
        QTableWidgetItem *nameItem    = new QTableWidgetItem;
        QTableWidgetItem *balanceItem = new QTableWidgetItem;
        
        qDebug() << "DaybookBalanceDialog::populateTableWidget() - " << "Serial: "  << debtorSerial;
        qDebug() << "DaybookBalanceDialog::populateTableWidget() - " << "Name: "    << debtorName;
        qDebug() << "DaybookBalanceDialog::populateTableWidget() - " << "Balance: " << debtorBalance << endl;

        if ( debtorBalance == "0" )
            continue;

        tableWidget->setRowCount(row + 1);

        serialItem->setText(debtorSerial);
        nameItem->setText(debtorName);
        balanceItem->setText(debtorBalance);

        tableWidget->setItem( row,   SERIAL,  serialItem );
        tableWidget->setItem( row,   NAME,    nameItem );
        tableWidget->setItem( row++, BALANCE, balanceItem );
    }

    progressDialog.setValue(row);
}
