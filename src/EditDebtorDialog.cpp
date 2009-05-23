#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

#include "EditDebtorDialog.h"

const int ID      = 0;
const int SERIAL  = 1;
const int NAME    = 2;
const int AGENT   = 3;
const int ADDRESS = 4;
const int AMOUNT  = 5;
const int PHONE   = 6;
const int DATE    = 7;

EditDebtorDialog::EditDebtorDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
    setupUi(this);

    QSqlQuery query;
    query.prepare("SELECT * FROM agent");
    if ( !query.exec() ) {
        QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
        msgbox->exec();

        return;
    }
    while ( query.next() ) {
         agentCombo->addItem( query.value(1).toString() );
         agentMap[query.value(0).toInt()] = query.value(1).toString();
    }
    agentCombo->setCurrentIndex(-1);

    tableWidget->hideColumn(ID);

    tableWidget->setColumnWidth( ID,      50);
    tableWidget->setColumnWidth( SERIAL,  50);
    tableWidget->setColumnWidth( NAME,    200);
    tableWidget->setColumnWidth( AGENT,   200);
    tableWidget->setColumnWidth( ADDRESS, 250);
    tableWidget->setColumnWidth( AMOUNT,  100);
    tableWidget->setColumnWidth( PHONE,   100);
    tableWidget->setColumnWidth( DATE,    100);

    connect( tableWidget,  SIGNAL( itemClicked(QTableWidgetItem *) ), this, SLOT( fetchItem(QTableWidgetItem *) ) );
    connect( saveButton,   SIGNAL(clicked()),                         this, SLOT(saveDebtor()) );
    connect( deleteButton, SIGNAL(clicked()),                         this, SLOT(deleteDebtor()) );
    connect( clearButton,  SIGNAL(clicked()),                         this, SLOT(clearDebtor()) );
    connect( serialEdit,   SIGNAL(textChanged(const QString&)),       this, SLOT(populateTableWidgetSerialEdit(const QString&)) );
}

void EditDebtorDialog::fetchItem(QTableWidgetItem *item) {
    currentRow = tableWidget->row(item);

    serialEdit->setText( tableWidget->item( currentRow,  SERIAL )->text() );
    nameEdit->setText( tableWidget->item( currentRow,    NAME )->text() );
    agentCombo->setCurrentIndex( agentCombo->findText( tableWidget->item( currentRow, AGENT )->text() ) );
    addressEdit->setText( tableWidget->item( currentRow, ADDRESS )->text() );
    amountEdit->setText( tableWidget->item( currentRow,  AMOUNT )->text() );
    phoneEdit->setText( tableWidget->item( currentRow,   PHONE )->text() );

    QDate date = QDate::fromString( tableWidget->item( currentRow, DATE)->text(), "yyyy-MM-dd" );
    dateCalendar->setSelectedDate(date);
}

void EditDebtorDialog::populateTableWidgetSerialEdit(const QString& serial) {
    QSqlQuery query;
    query.prepare("SELECT * FROM debtor WHERE serial = :serial");
    query.bindValue( ":serial", serial );

    if ( !query.exec() ) {
        QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
        msgbox->exec();

        return;
    }

    tableWidget->setRowCount(query.size());

    int row = 0;

    while ( query.next() ) {
        QTableWidgetItem *idItem      = new QTableWidgetItem;
        QTableWidgetItem *serialItem  = new QTableWidgetItem;
        QTableWidgetItem *nameItem    = new QTableWidgetItem;
        QTableWidgetItem *agentItem   = new QTableWidgetItem;
        QTableWidgetItem *addressItem = new QTableWidgetItem;
        QTableWidgetItem *amountItem  = new QTableWidgetItem;
        QTableWidgetItem *phoneItem   = new QTableWidgetItem;
        QTableWidgetItem *dateItem    = new QTableWidgetItem;

        idItem->setText( query.value(ID).toString() );
        serialItem->setText( query.value(SERIAL).toString() );
        nameItem->setText( query.value(NAME).toString() );
        agentItem->setText( agentMap[ query.value(AGENT).toString().toInt() ] );
        addressItem->setText( query.value(ADDRESS).toString() );
        amountItem->setText( query.value(AMOUNT).toString() );
        phoneItem->setText( query.value(PHONE).toString() );
        dateItem->setText( query.value(DATE).toString() );

        tableWidget->setItem( row,   ID,      idItem );
        tableWidget->setItem( row,   SERIAL,  serialItem );
        tableWidget->setItem( row,   NAME,    nameItem );
        tableWidget->setItem( row,   AGENT,   agentItem );
        tableWidget->setItem( row,   ADDRESS, addressItem );
        tableWidget->setItem( row,   AMOUNT,  amountItem );
        tableWidget->setItem( row,   PHONE,   phoneItem );
        tableWidget->setItem( row++, DATE,    dateItem );
    }
}

void EditDebtorDialog::saveDebtor() {
    if ( "" == serialEdit->text() || "" == nameEdit->text() || -1 == agentCombo->currentIndex() || "" == addressEdit->toPlainText() || "" == amountEdit->text() ) {
        QMessageBox *msgbox = new QMessageBox(
            QMessageBox::Warning, "Incomplete Fields",
            "All fields are to be filled.", QMessageBox::Ok );
        msgbox->exec();

        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE debtor SET serial = :serial, name = :name, agent_id = :agent_id, address = :address, amount = :amount, phone = :phone, date = :date WHERE id = :id");

    query.bindValue( ":id",       tableWidget->item( currentRow, ID )->text() );
    query.bindValue( ":serial",   serialEdit->text() );
    query.bindValue( ":name",     nameEdit->text() );
    query.bindValue( ":agent_id", agentMap.key( agentCombo->currentText() ) );
    query.bindValue( ":address",  addressEdit->toPlainText() );
    query.bindValue( ":amount",   amountEdit->text() );
    query.bindValue( ":phone",    phoneEdit->text() );
    query.bindValue( ":date",     dateCalendar->selectedDate().toString(Qt::ISODate) );

    if ( !query.exec() ) {
        QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
        msgbox->exec();

        return;
    }
    
    clearDebtor();
}

void EditDebtorDialog::deleteDebtor() {
    if ( QMessageBox::No == QMessageBox::warning( this, tr("Delete debtor?"), "Are you sure you want to delete debtor details?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) )
        return;

    QSqlQuery query;
    query.prepare("DELETE FROM debtor WHERE id = :id");
    query.bindValue( ":id", tableWidget->item( currentRow, ID )->text() );

    if ( !query.exec() ) {
        QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
        msgbox->exec();

        return;
    }

    clearDebtor();
}

void EditDebtorDialog::clearDebtor() {
    serialEdit->clear();
    nameEdit->clear();
    agentCombo->setCurrentIndex(-1);
    addressEdit->clear();
    amountEdit->clear();
    phoneEdit->clear();
    tableWidget->clearContents();

    serialEdit->setFocus();
}
