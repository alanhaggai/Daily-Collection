#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

#include "edittransactiondialog.h"

const int ID          = 0;
const int DATE        = 1;
const int TRANSACTION = 2;

EditTransactionDialog::EditTransactionDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
    setupUi(this);

    tableWidget->hideColumn(ID);

    connect( tableWidget, SIGNAL( itemClicked(QTableWidgetItem *) ), this, SLOT( fetchItem(QTableWidgetItem *) ) );
    //connect( saveButton,   SIGNAL(clicked()),                         this, SLOT(saveTransaction()) );
    //connect( deleteButton, SIGNAL(clicked()),                         this, SLOT(deleteTransaction()) );
    connect( clearButton,  SIGNAL(clicked()),                         this, SLOT(clearTransaction()) );
    connect( serialEdit,  SIGNAL(textChanged(const QString&)),       this, SLOT(populateTableWidgetSerialEdit(const QString&)) );
}

void EditTransactionDialog::populateTableWidgetSerialEdit(const QString& serial) {
    QSqlQuery query;
    query.prepare("SELECT transaction.id, transaction.date, transaction.paid, transaction.debtor_id FROM debtor, transaction WHERE debtor.id = transaction.debtor_id AND debtor.serial = :debtor_serial ORDER BY transaction.date");
    query.bindValue( ":debtor_serial", serial );

    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    int row = 0;

    while ( query.next() ) {
        QTableWidgetItem *idItem          = new QTableWidgetItem;
        QTableWidgetItem *dateItem        = new QTableWidgetItem;
        QTableWidgetItem *transactionItem = new QTableWidgetItem;

        qDebug() << "EditTransactionDialog::populateTableWidget() - " << "ID: "          << query.value(ID).toString();
        qDebug() << "EditTransactionDialog::populateTableWidget() - " << "Date: "        << query.value(DATE).toString();
        qDebug() << "EditTransactionDialog::populateTableWidget() - " << "Transaction: " << query.value(TRANSACTION).toString();

        idItem->setText( query.value(ID).toString() );
        dateItem->setText( query.value(DATE).toString() );
        transactionItem->setText( query.value(TRANSACTION).toString() );

        tableWidget->setRowCount(row + 1);

        tableWidget->setItem( row,   ID,          idItem );
        tableWidget->setItem( row,   DATE,        dateItem );
        tableWidget->setItem( row++, TRANSACTION, transactionItem );
    }

    query.prepare("SELECT name FROM debtor WHERE serial = :serial");
    query.bindValue( ":serial", serial );

    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    query.next();
    nameEdit->setText( query.value(0).toString() );
    qDebug() << query.value(1).toString();
}

void EditTransactionDialog::fetchItem(QTableWidgetItem *item) {
    currentRow = tableWidget->row(item);

    QDate date = QDate::fromString( tableWidget->item( currentRow, DATE)->text(), "yyyy-MM-dd" );
    dateCalendar->setSelectedDate(date);
    transactionEdit->setText( tableWidget->item( currentRow, TRANSACTION )->text() );
}

void EditTransactionDialog::clearTransaction() {
    serialEdit->clear();
    nameEdit->clear();
    transactionEdit->clear();
    tableWidget->clearContents();

    serialEdit->setFocus();
}
