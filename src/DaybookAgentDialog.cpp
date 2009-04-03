#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "DaybookAgentDialog.h"

const int SERIAL  = 0;
const int NAME    = 1;
const int AMOUNT  = 2;
const int PAID    = 3;
const int BALANCE = 4;

DaybookAgentDialog::DaybookAgentDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
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
    tableWidget->setColumnWidth( AMOUNT,  70);
    tableWidget->setColumnWidth( PAID,    70);
    tableWidget->setColumnWidth( BALANCE, 70);

    connect( agentCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(populateTableWidget(int)) );
}

void DaybookAgentDialog::populateTableWidget(int currentIndex) {
    int agentId = agentMap.key( agentCombo->itemText(currentIndex) );

    QSqlQuery query;
    query.prepare("SELECT debtor.serial, debtor.name, debtor.amount, SUM(transaction.paid) FROM debtor, transaction WHERE debtor.agent_id = :agentId AND debtor.id = transaction.debtor_id GROUP BY debtor.serial ORDER BY debtor.serial");
    query.bindValue( ":agent_id", agentId );

    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    tableWidget->setRowCount(query.size());

    int row = 0;

    int totalAmount  = 0;
    int totalPaid    = 0;
    int totalBalance = 0;

    while ( query.next() ) {
        QString debtorSerial  = query.value(SERIAL).toString();
        QString debtorName    = query.value(NAME).toString();
        QString debtorAmount  = query.value(AMOUNT).toString();
        QString debtorPaid    = query.value(PAID).toString();
        QString debtorBalance = QString::number( query.value(AMOUNT).toInt() - query.value(PAID).toInt() );

        QTableWidgetItem *serialItem  = new QTableWidgetItem;
        QTableWidgetItem *nameItem    = new QTableWidgetItem;
        QTableWidgetItem *amountItem  = new QTableWidgetItem;
        QTableWidgetItem *paidItem    = new QTableWidgetItem;
        QTableWidgetItem *balanceItem = new QTableWidgetItem;

        qDebug() << "DaybookAgentDialog::populateTableWidget() - " << "Serial: "  + debtorSerial;
        qDebug() << "DaybookAgentDialog::populateTableWidget() - " << "Name: "    + debtorName;
        qDebug() << "DaybookAgentDialog::populateTableWidget() - " << "Amount: "  + debtorAmount;
        qDebug() << "DaybookAgentDialog::populateTableWidget() - " << "Paid: "    + debtorPaid;
        qDebug() << "DaybookAgentDialog::populateTableWidget() - " << "Balance: " + debtorBalance;

        serialItem->setText(debtorSerial);
        nameItem->setText(debtorName);
        amountItem->setText(debtorAmount);
        paidItem->setText(debtorPaid);
        balanceItem->setText(debtorBalance);

        tableWidget->setItem( row,   SERIAL,  serialItem );
        tableWidget->setItem( row,   NAME,    nameItem );
        tableWidget->setItem( row,   AMOUNT,  amountItem );
        tableWidget->setItem( row,   PAID,    paidItem );
        tableWidget->setItem( row++, BALANCE, balanceItem );

        totalAmount  += debtorAmount.toInt();
        totalPaid    += debtorPaid.toInt();
        totalBalance += debtorBalance.toInt();
    }

    totalAmountEdit->setText( QString::number(totalAmount) );
    totalPaidEdit->setText( QString::number(totalPaid) );
    totalBalanceEdit->setText( QString::number(totalBalance) );
}
