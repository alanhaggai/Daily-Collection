#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QProgressDialog>

#include "daybookbalancedialog.h"

const int SERIAL  = 0;
const int NAME    = 1;
const int BALANCE = 2;

DaybookBalanceDialog::DaybookBalanceDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
    setupUi(this);

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
    query.prepare("SELECT DISTINCT debtor.serial, debtor.name, transaction.paid FROM debtor, agent, transaction WHERE transaction.agent_id = agent.id AND debtor.id = transaction.debtor_id AND transaction.date <= :transactionDate AND agent.id = :agentId GROUP BY debtor.serial ORDER BY debtor.serial");

    query.bindValue( ":agentId",         agentMap.key(agentCombo->currentText()) );
    query.bindValue( ":transactionDate", dateCalendar->selectedDate().toString(Qt::ISODate) );

    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    QProgressDialog progressDialog( "Retrieving data...", "Abort Fetch", 0, query.size(), this );
    progressDialog.setWindowModality(Qt::WindowModal);

    int row = 0, counter = 0;

    while ( query.next() ) {
        progressDialog.setValue(counter++);
        qApp->processEvents();

        if ( progressDialog.wasCanceled() )
            break;

        QTableWidgetItem *serialItem  = new QTableWidgetItem;
        QTableWidgetItem *nameItem    = new QTableWidgetItem;
        QTableWidgetItem *balanceItem = new QTableWidgetItem;
        
        QSqlQuery queryBalance;
        queryBalance.prepare("SELECT debtor.amount, SUM(transaction.paid) FROM debtor, transaction WHERE debtor.id = transaction.debtor_id AND transaction.date <= :date AND debtor.serial = :serial GROUP BY debtor.id");

        queryBalance.bindValue( ":date",   dateCalendar->selectedDate().toString(Qt::ISODate) );
        queryBalance.bindValue( ":serial", query.value(SERIAL).toString() );

        if ( !queryBalance.exec() ) {
            qDebug() << queryBalance.lastError();
            qFatal("Failed to execute query.");
        }

        queryBalance.next();

        int balance = queryBalance.value(0).toInt() - queryBalance.value(1).toInt();

        qDebug() << "DaybookBalanceDialog::populateTableWidget() - " << "Serial: "  << query.value(SERIAL).toString();
        qDebug() << "DaybookBalanceDialog::populateTableWidget() - " << "Name: "    << query.value(NAME).toString();
        qDebug() << "DaybookBalanceDialog::populateTableWidget() - " << "Balance: " << balance << endl;

        if ( balance == 0 )
            continue;

        tableWidget->setRowCount(row + 1);

        serialItem->setText( query.value(SERIAL).toString() );
        nameItem->setText( query.value(NAME).toString() );
        balanceItem->setText( QString::number(balance) );

        tableWidget->setItem( row,   SERIAL,  serialItem );
        tableWidget->setItem( row,   NAME,    nameItem );
        tableWidget->setItem( row++, BALANCE, balanceItem );
    }

    progressDialog.setValue(row);
}
