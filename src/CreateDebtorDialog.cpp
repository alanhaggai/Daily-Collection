/*!
 * \file  CreateDebtorDialog.cpp
 * \brief Provides interface for the creation of debtors
 *
 * \ingroup Create
 */

#include "CreateDebtorDialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

/*!
 * Setup UI and connect save_button to slot SaveDebtor. Populate agent_combo and
 * agent_map with names and ids of agents.
 */
CreateDebtorDialog::CreateDebtorDialog(QDialog* parent)
    : QDialog( parent, Qt::Tool )
{
    setupUi(this);

    // Get all agent ids and names from agent table
    QSqlQuery query;
    query.prepare("SELECT id, name FROM agent");
    if ( !query.exec() ) {
        // Execution of query failed
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }
    // Loop through the results
    while ( query.next() ) {
        int agent_id       = query.value(0).toInt();
        QString agent_name = query.value(1).toString();

        agent_combo->addItem(agent_name);
        // Populate agent_map with id of agent as key and name as value
        agent_map[agent_id] = agent_name;
    }
    // Set agent_combo's default index to -1 so that no names will be selected
    // by default
    agent_combo->setCurrentIndex(-1);

    connect( save_button, SIGNAL(clicked()), this, SLOT(SaveDebtor()) );
}

/*!
 * Check if Line Edits are empty or not. If they are empty, fire up a message
 * box and return from the method. Else, fetch data from Line Edits and Combo
 * Box, and insert into database. Clear the Line Edits and set focus on
 * serial_edit.
 */
void
CreateDebtorDialog::SaveDebtor() {
    // Check if serial, name, agent, address and amount have been entered or
    // not
    if (    "" == serial_edit->text()         ||
            "" == name_edit->text()           ||
            -1 == agent_combo->currentIndex() ||
            "" == address_edit->toPlainText() ||
            "" == amount_edit->text() )
    {
        QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Information, "Incomplete Fields",
                "All fields are to be filled.", QMessageBox::Ok );
        msgbox->show();  // Popup message box to let the user know about it
        return;
    }

    // Output debug information
    qDebug() << "CreateDebtorDialog::SaveDebtor() - " << "Serial: " <<
            serial_edit->text();
    qDebug() << "CreateDebtorDialog::SaveDebtor() - " << "Name: " <<
            name_edit->text();
    qDebug() << "CreateDebtorDialog::SaveDebtor() - " << "Agent: " <<
            agent_combo->currentText();
    qDebug() << "CreateDebtorDialog::SaveDebtor() - " << "Address: " <<
            address_edit->toPlainText();
    qDebug() << "CreateDebtorDialog::SaveDebtor() - " << "Amount: " <<
            amount_edit->text();
    qDebug() << "CreateDebtorDialog::SaveDebtor() - " << "Phone: " <<
            phone_edit->text();
    qDebug() << "CreateDebtorDialog::SaveDebtor() - " << "Date: " <<
            date_calendar->selectedDate().toString(Qt::ISODate);

    // Insert serial, name, agent_id, address, amount, phone and date into table
    // debtor
    QSqlQuery query;
    query.prepare("INSERT INTO debtor( serial, name, agent_id, address, amount,"
            "phone, date ) VALUES ( :serial, :name, :agent_id, :address,\
            :amount, :phone, :date )");
    query.bindValue( ":serial",   serial_edit->text() );
    query.bindValue( ":name",     name_edit->text() );
    query.bindValue( ":agent_id", agent_map.key( agent_combo->currentText() ) );
    query.bindValue( ":address",  address_edit->toPlainText() );
    query.bindValue( ":amount",   amount_edit->text() );
    query.bindValue( ":phone",    phone_edit->text() );
    query.bindValue( ":date",
        date_calendar->selectedDate().toString(Qt::ISODate) );

    if ( !query.exec() ) {
        // Query failed to execute
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    // Clear Line Edits and Combo Box
    serial_edit->clear();
    name_edit->clear();
    agent_combo->setCurrentIndex(-1);
    address_edit->clear();
    amount_edit->clear();
    phone_edit->clear();
    date_calendar->showToday();

    serial_edit->setFocus();
}