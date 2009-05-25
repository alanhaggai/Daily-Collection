#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#include "EditTransactionDialog.h"

const int ID          = 0;
const int DATE        = 1;
const int TRANSACTION = 2;

EditTransactionDialog::EditTransactionDialog(QDialog *parent) : QDialog(parent) {
    setupUi(this);

    tableWidget->hideColumn(ID);

    connect( tableWidget, SIGNAL( itemClicked(QTableWidgetItem *) ), this, SLOT( fetchItem(QTableWidgetItem *) ) );
    connect( saveButton,   SIGNAL(clicked()),                         this, SLOT(saveTransaction()) );
    //connect( deleteButton, SIGNAL(clicked()),                         this, SLOT(deleteTransaction()) );
    connect( clearButton,  SIGNAL(clicked()),                         this, SLOT(clearTransaction()) );
    connect( serialEdit,  SIGNAL(textChanged(const QString&)),       this, SLOT(populateTableWidgetSerialEdit(const QString&)) );
}

void EditTransactionDialog::saveTransaction() {
    if ( "" == serialEdit->text() || "" == nameEdit->text() || "" == transactionEdit->text() ) {
            QMessageBox *msgbox = new QMessageBox(
                QMessageBox::Warning, "Incomplete Fields",
                "All fields are to be filled.", QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    QSqlQuery query;
    query.prepare("UPDATE transactions SET date = :date, paid = :paid WHERE id = :id");

    query.bindValue( ":date", dateCalendar->selectedDate().toString(Qt::ISODate) );
    query.bindValue( ":paid", transactionEdit->text() );
    query.bindValue( ":id",   transactionId );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    transactionEdit->clear();
    populateTableWidgetSerialEdit( serialEdit->text() );

    nameEdit->setFocus();
}

void EditTransactionDialog::populateTableWidgetSerialEdit(const QString& serial) {
    QSqlQuery query;
    query.prepare("SELECT transactions.id, transactions.date, transactions.paid, transactions.debtor_id FROM debtor, transactions WHERE debtor.id = transactions.debtor_id AND debtor.serial = :debtor_serial ORDER BY transactions.date");
    query.bindValue( ":debtor_serial", serial );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }
    qint32 row = 0;

    while ( query.next() ) {
            tableWidget->setRowCount( row + 1 );

            QTableWidgetItem *idItem          = new QTableWidgetItem;
            QTableWidgetItem *dateItem        = new QTableWidgetItem;
            QTableWidgetItem *transactionItem = new QTableWidgetItem;

            idItem->setText( query.value(ID).toString() );
            dateItem->setText( query.value(DATE).toString() );
            transactionItem->setText( query.value(TRANSACTION).toString() );

            tableWidget->setItem( row,   ID,          idItem );
            tableWidget->setItem( row,   DATE,        dateItem );
            tableWidget->setItem( row++, TRANSACTION, transactionItem );
        }

    query.prepare("SELECT name FROM debtor WHERE serial = :serial");
    query.bindValue( ":serial", serial );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    query.next();
    nameEdit->setText( query.value(0).toString() );
}

void EditTransactionDialog::fetchItem(QTableWidgetItem *item) {
    currentRow = tableWidget->row(item);

    transactionId = tableWidget->item( currentRow, ID )->text().toInt();
    QDate date = QDate::fromString( tableWidget->item( currentRow, DATE)->text(), "yyyy-MM-dd" );
    dateCalendar->setSelectedDate(date);
    transactionEdit->setText( tableWidget->item( currentRow, TRANSACTION )->text() );
}

void EditTransactionDialog::clearTransaction() {
    serialEdit->clear();
    nameEdit->clear();
    transactionEdit->clear();
    tableWidget->clearContents();
    tableWidget->setRowCount(0);

    serialEdit->setFocus();
}
