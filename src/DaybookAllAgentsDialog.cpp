#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "DaybookAllAgentsDialog.h"

const int NAME           = 0;
const int AMOUNTGIVEN    = 1;
const int AMOUNTREMITTED = 2;
const int BALANCE        = 3;

DaybookAllAgentsDialog::DaybookAllAgentsDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
    setupUi(this);

    populateTableWidget();
}

void DaybookAllAgentsDialog::populateTableWidget() {
    tableWidget->setColumnWidth( NAME,  200);
    tableWidget->setColumnWidth( AMOUNTGIVEN,    150);
    tableWidget->setColumnWidth( AMOUNTREMITTED,  150);
    tableWidget->setColumnWidth( BALANCE,  150);

    QSqlQuery agentQuery, debtorQuery, transactionQuery;
    agentQuery.prepare("SELECT id, name FROM agent");

    if ( !agentQuery.exec() ) {
        qDebug() << agentQuery.lastError();
        qFatal("Failed to execute query.");
    }

    tableWidget->setRowCount( agentQuery.size() );
    
    int row = 0;

    while ( agentQuery.next() ) {
        QString agentId   = agentQuery.value(0).toString();
        QString agentName = agentQuery.value(1).toString();

        debtorQuery.prepare("SELECT SUM(amount) FROM debtor WHERE agent_id = :agent_id");
        debtorQuery.bindValue( ":agent_id", agentId );

        if ( !debtorQuery.exec() ) {
            qDebug() << debtorQuery.lastError();
            qFatal("Failed to execute query.");
        }

        debtorQuery.next();
        int agentAmountGiven = debtorQuery.value(0).toInt();

        transactionQuery.prepare("SELECT SUM(paid) FROM transaction where agent_id = :agent_id");
        transactionQuery.bindValue( ":agent_id", agentId );

        if ( !transactionQuery.exec() ) {
            qDebug() << transactionQuery.lastError();
            qFatal("Failed to execute query.");
        }

        transactionQuery.next();
        int agentAmountRemitted = transactionQuery.value(0).toInt();
        int agentBalance        = agentAmountGiven - agentAmountRemitted;

        QTableWidgetItem *nameItem           = new QTableWidgetItem;
        QTableWidgetItem *amountGivenItem    = new QTableWidgetItem;
        QTableWidgetItem *amountRemittedItem = new QTableWidgetItem;
        QTableWidgetItem *balanceItem        = new QTableWidgetItem;

        qDebug() << "DaybookAllAgentsDialog::populateTableWidget() - " << "Name: "            << agentName;
        qDebug() << "DaybookAllAgentsDialog::populateTableWidget() - " << "Amount Given: "    << agentAmountGiven;
        qDebug() << "DaybookAllAgentsDialog::populateTableWidget() - " << "Amount Remitted: " << agentAmountRemitted;
        qDebug() << "DaybookAllAgentsDialog::populateTableWidget() - " << "Balance: "         << agentBalance;

        nameItem->setText(agentName);
        amountGivenItem->setText( QString::number(agentAmountGiven) );
        amountRemittedItem->setText( QString::number(agentAmountRemitted) );
        balanceItem->setText( QString::number(agentBalance) );

        tableWidget->setItem( row,   NAME,           nameItem );
        tableWidget->setItem( row,   AMOUNTGIVEN,    amountGivenItem );
        tableWidget->setItem( row,   AMOUNTREMITTED, amountRemittedItem );
        tableWidget->setItem( row++, BALANCE,        balanceItem );
    }

}
