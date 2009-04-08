/*!
 * \file  CreateAgentDialog.cpp
 * \brief Provides interface for the creation of agents.
 *
 * \ingroup Create
 */

#include "CreateAgentDialog.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

//! Setup UI and connect save_button to slot SaveAgent.
CreateAgentDialog::CreateAgentDialog(QDialog* parent)
        : QDialog(parent, Qt::Tool)
{
    setupUi(this);
    connect( save_button, SIGNAL(clicked()), this, SLOT(SaveAgent()) );
}

/*!
 * Check if Line Edits are empty or not. If they are empty, popup a message box
 * and return from the method. Else, fetch data from Line Edits and insert into
 * agent table. Clear the Line Edits and set focus on name_edit.
 */
void
CreateAgentDialog::SaveAgent() {
    QString agent_name    = name_edit->text();            // Agent name
    QString agent_address = address_edit->toPlainText();  // Agent address
    QString agent_phone   = phone_edit->text();           // Agent phone

    // Check if name and address have been entered or not
    if ( "" == agent_name || "" == agent_address ) {
        QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Information, "Incomplete Fields",
                "All fields are to be filled.", QMessageBox::Ok );
        msgbox->show();  // Fire up a message box
        return;
    }

    // Output debug information
    qDebug() << "CreateAgentDialog::SaveAgent() - " << "Name: " << agent_name;
    qDebug() << "CreateAgentDialog::SaveAgent() - " << "Address: " <<
            agent_address;
    qDebug() << "CreateAgentDialog::SaveAgent() - " << "Phone: " << agent_phone;

    // Insert name, address and phone of agent into the database
    QSqlQuery query;
    query.prepare("INSERT INTO agent( name, address, phone )"
            "VALUES ( :name, :address, :phone )");
    query.bindValue( ":name",    agent_name );
    query.bindValue( ":address", agent_address );
    query.bindValue( ":phone",   agent_phone );

    if ( !query.exec() ) {
        // Query execution failed: Exit with fatal error
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    // Clear Line Edit controls and set focus on name_edit
    name_edit->clear();
    address_edit->clear();
    phone_edit->clear();
    name_edit->setFocus();
}
