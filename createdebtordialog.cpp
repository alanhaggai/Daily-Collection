#include <QSqlQuery>

#include "createdebtordialog.h"

CreateDebtorDialog::CreateDebtorDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
    setupUi(this);

    QSqlQuery query;
    query.prepare("SELECT * FROM agent");
    if ( !query.exec() ) {
        qFatal("Failed to execute query.");
    }
    while ( query.next() ) {
         agentCombo->addItem(
             "(" + query.value(0).toString() + ") " + query.value(1).toString()
         );
    }
    agentCombo->setCurrentIndex(-1);

    connect( saveButton, SIGNAL(clicked()), this, SLOT(save()) );
}

void CreateDebtorDialog::save(void) {
    qDebug() << "CreateDebtorDialog::save() - " << "Serial: "  + serialEdit->text();
    qDebug() << "CreateDebtorDialog::save() - " << "Name: "    + nameEdit->text();
    qDebug() << "CreateDebtorDialog::save() - " << "Agent:"    + agentCombo->currentText();
    qDebug() << "CreateDebtorDialog::save() - " << "Address: " + addressEdit->toPlainText();
    qDebug() << "CreateDebtorDialog::save() - " << "Amount: "  + amountEdit->text();
    qDebug() << "CreateDebtorDialog::save() - " << "Phone: "   + phoneEdit->text();
    qDebug() << "CreateDebtorDialog::save() - " << "Date: "    + dateCalendar->selectedDate().toString(Qt::ISODate);

    QString agent = agentCombo->currentText();
    QRegExp re("\\((\\d+)\\)");
    re.indexIn(agent);
    QString agentId = re.cap(1);

    QSqlQuery query;
    query.prepare("INSERT INTO debtor( serial, name, agent_id, address, amount, paid, phone, date )"
            "VALUES ( :serial, :name, :agent_id, :address, :amount, :paid, :phone, :date )");
    query.bindValue( ":serial",   serialEdit->text() );
    query.bindValue( ":name",     nameEdit->text() );
    query.bindValue( ":agent_id", agentId );
    query.bindValue( ":address",  addressEdit->toPlainText() );
    query.bindValue( ":amount",   amountEdit->text() );
    query.bindValue( ":paid",     0 );
    query.bindValue( ":phone",    phoneEdit->text() );
    query.bindValue( ":date",     dateCalendar->selectedDate().toString(Qt::ISODate) );

    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    serialEdit->clear();
    nameEdit->clear();
    agentCombo->setCurrentIndex(-1);
    addressEdit->clear();
    amountEdit->clear();
    phoneEdit->clear();
    dateCalendar->showToday();
}
