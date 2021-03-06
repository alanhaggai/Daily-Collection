#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#include "EditAgentDialog.h"

enum {
    ID,
    NAME,
    ADDRESS,
    PHONE,
};

EditAgentDialog::EditAgentDialog(QDialog *parent) : QDialog(parent) {
    setupUi(this);

    tableWidget->hideColumn(ID);
    populateTableWidget();

    connect( tableWidget, SIGNAL( itemClicked(QTableWidgetItem *) ), this,
            SLOT( fetchItem(QTableWidgetItem *) ) );
    connect( saveButton,  SIGNAL(clicked()),                         this,
            SLOT(save()) );
}

void EditAgentDialog::populateTableWidget() {
    QSqlQuery query;
    query.prepare("SELECT * FROM agent");

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query execution failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    tableWidget->setColumnWidth( ID,      50);
    tableWidget->setColumnWidth( NAME,    200);
    tableWidget->setColumnWidth( ADDRESS, 200);
    tableWidget->setColumnWidth( PHONE,   150);

    qint32 row = 0;

    while ( query.next() ) {
            tableWidget->setRowCount( row + 1 );

            QTableWidgetItem *idItem      = new QTableWidgetItem;
            QTableWidgetItem *nameItem    = new QTableWidgetItem;
            QTableWidgetItem *addressItem = new QTableWidgetItem;
            QTableWidgetItem *phoneItem   = new QTableWidgetItem;

            idItem->setText( query.value(ID).toString() );
            nameItem->setText( query.value(NAME).toString() );
            addressItem->setText( query.value(ADDRESS).toString() );
            phoneItem->setText( query.value(PHONE).toString() );

            tableWidget->setItem( row,   ID,      idItem );
            tableWidget->setItem( row,   NAME,    nameItem );
            tableWidget->setItem( row,   ADDRESS, addressItem );
            tableWidget->setItem( row++, PHONE,   phoneItem );
        }
}

void EditAgentDialog::fetchItem(QTableWidgetItem *item) {
    currentRow = tableWidget->row(item);

    nameEdit->setText( tableWidget->item( currentRow,    NAME )->text() );
    addressEdit->setText( tableWidget->item( currentRow, ADDRESS )->text() );
    phoneEdit->setText( tableWidget->item( currentRow,   PHONE )->text() );
}

void EditAgentDialog::save() {
    if ( "" == nameEdit->text() || "" == addressEdit->toPlainText()
            || "" == phoneEdit->text() ) {
            QMessageBox *msgbox = new QMessageBox(
                QMessageBox::Warning, "Incomplete Fields",
                "All fields are to be filled.", QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    QSqlQuery query;
    query.prepare("UPDATE agent SET name = :name, address = :address, \
            phone = :phone WHERE id = :id");

    query.bindValue( ":id",      tableWidget->item( currentRow, ID )->text() );
    query.bindValue( ":name",    nameEdit->text() );
    query.bindValue( ":address", addressEdit->toPlainText() );
    query.bindValue( ":phone",   phoneEdit->text() );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query execution failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    nameEdit->clear();
    addressEdit->clear();
    phoneEdit->clear();

    populateTableWidget();

    nameEdit->setFocus();
}
