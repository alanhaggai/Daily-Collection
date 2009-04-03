#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

#include "createdebtordialog.h"

CreateDebtorDialog::CreateDebtorDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
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

    connect( saveButton, SIGNAL(clicked()), this, SLOT(save()) );
}

void CreateDebtorDialog::save(void) {
    if ( "" == serialEdit->text() || "" == nameEdit->text() || -1 == agentCombo->currentIndex() || "" == addressEdit->toPlainText() || "" == amountEdit->text() ) {
        QMessageBox *msgbox = new QMessageBox(
            QMessageBox::Information, "Incomplete Fields",
            "All fields are to be filled.", QMessageBox::Ok );
        msgbox->show();
        return;
    }

    qDebug() << "CreateDebtorDialog::save() - " << "Serial: "  + serialEdit->text();
    qDebug() << "CreateDebtorDialog::save() - " << "Name: "    + nameEdit->text();
    qDebug() << "CreateDebtorDialog::save() - " << "Agent: "   + agentCombo->currentText();
    qDebug() << "CreateDebtorDialog::save() - " << "Address: " + addressEdit->toPlainText();
    qDebug() << "CreateDebtorDialog::save() - " << "Amount: "  + amountEdit->text();
    qDebug() << "CreateDebtorDialog::save() - " << "Phone: "   + phoneEdit->text();
    qDebug() << "CreateDebtorDialog::save() - " << "Date: "    + dateCalendar->selectedDate().toString(Qt::ISODate);

    QString agent = agentCombo->currentText();
    QRegExp re("\\((\\d+)\\)");
    re.indexIn(agent);
    QString agentId = re.cap(1);

    QSqlQuery query;
    query.prepare("INSERT INTO debtor( serial, name, agent_id, address, amount, phone, date )"
            "VALUES ( :serial, :name, :agent_id, :address, :amount, :phone, :date )");
    query.bindValue( ":serial",   serialEdit->text() );
    query.bindValue( ":name",     nameEdit->text() );
    query.bindValue( ":agent_id", agentMap.key( agentCombo->currentText() ) );
    query.bindValue( ":address",  addressEdit->toPlainText() );
    query.bindValue( ":amount",   amountEdit->text() );
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
