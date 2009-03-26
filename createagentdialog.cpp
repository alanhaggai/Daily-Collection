#include "createagentdialog.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

CreateAgentDialog::CreateAgentDialog(QDialog *parent) : QDialog(parent, Qt::Tool) {
    setupUi(this);

    connect( saveButton, SIGNAL(clicked()), this, SLOT(save()) );
}

void CreateAgentDialog::save() {
    if ( nameEdit->text() == "" || addressEdit->toPlainText() == "" || phoneEdit->text() == "" ) {
        QMessageBox *msgbox = new QMessageBox(
            QMessageBox::Information, "Incomplete Fields",
            "All fields are to be filled.", QMessageBox::Ok );
        msgbox->show();
        return;
    }

    qDebug() << "CreateAgentDialog::save() - " << "Name: "    + nameEdit->text();
    qDebug() << "CreateAgentDialog::save() - " << "Address: " + addressEdit->toPlainText();
    qDebug() << "CreateAgentDialog::save() - " << "Phone: "   + phoneEdit->text();

    QSqlQuery query;
    query.prepare("INSERT INTO agent( name, address, phone )"
            "VALUES ( :name, :address, :phone )");
    query.bindValue( ":name",    nameEdit->text() );
    query.bindValue( ":address", addressEdit->toPlainText() );
    query.bindValue( ":phone",   phoneEdit->text() );

    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }
    else {
        qDebug() << "Successfully executed query.";
    }

    nameEdit->clear();
    addressEdit->clear();
    phoneEdit->clear();
    nameEdit->setFocus();
}
