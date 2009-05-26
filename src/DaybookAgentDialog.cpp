/*!
 * \file  DaybookAgentDialog.cpp
 * \brief Displays daybook of each agent
 *
 * \ingroup Daybook
 */

#include "DaybookAgentDialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

//! Enumerator constants for representing Table Widget columns
enum {
    SERIAL,
    NAME,
    AMOUNT,
    PAID,
    BALANCE,
};

/*!
 * Constructor that creates a window for displaying daybook of agents.
 * UI is setup. Agent ids and names are selected from agent table. Agent Combo
 * Box is populated with agent names and mapping of agent names to agent ids is
 * done. Table Widget column widths are set.
 */
DaybookAgentDialog::DaybookAgentDialog(QDialog* parent) :
        QDialog(parent) {
    setupUi(this);

    // Get all agent ids and names from agent table
    QSqlQuery query;
    query.prepare("SELECT id, name FROM agent");

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query execution failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    // Cycle through result rows
    while ( query.next() ) {
            // Add agent name to Combo Box
            agent_combo->addItem( query.value(1).toString() );

            // Create a mapping with id as key and name as value
            agent_map[ query.value(0).toInt() ] = query.value(1).toString();
        }

    agent_combo->setCurrentIndex(-1);  // Clear Combo Box selection

    // Set column widths for Table Widget
    table_widget->setColumnWidth( SERIAL,  50);
    table_widget->setColumnWidth( NAME,    200);
    table_widget->setColumnWidth( AMOUNT,  70);
    table_widget->setColumnWidth( PAID,    70);
    table_widget->setColumnWidth( BALANCE, 70);

    connect( agent_combo, SIGNAL( currentIndexChanged(int) ), this,
            SLOT( PopulateTableWidget(int) ) );
}

/*!
 * Get id of currently selected agent name. Calculate total amount given to a
 * debtor, total amount paid by the debtor and balance amount to be paid by the
 * debtor. Calculate the grand total of the above amounts and display.
 */
void
DaybookAgentDialog::PopulateTableWidget(int current_index) {
    // Get the id of the agent whose name has been selected in the Combo Box
    int agent_id = agent_map.key( agent_combo->itemText(current_index) );

    // Calculate the total sum paid by a debtor under the agent whose name has
    // been selected in the Combo Box
    QSqlQuery query;
    query.prepare("SELECT debtor.serial, debtor.name, debtor.amount,\
        SUM(transactions.paid) FROM debtor, transactions WHERE debtor.agent_id\
        = :agent_id AND debtor.id = transactions.debtor_id GROUP BY\
        debtor.serial ORDER BY debtor.serial");
    query.bindValue( ":agent_id", agent_id );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query execution failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    qint32 row = 0;  // Row count

    int total_amount  = 0;  // Total amount received by debtors from an agent
    int total_paid    = 0;  // Total amount paid by debtors to an agent
    int total_balance = 0;  // Equals: total_amount - total_paid

    // Loop through result rows
    while ( query.next() ) {
            // Set row count to the query result size
            table_widget->setRowCount( row + 1 );

            QString debtor_serial;   // Debtor serial number
            QString debtor_name;     // Debtor name
            QString debtor_amount;   // Amount received by debtor
            QString debtor_paid;     // Total amount paid by debtor
            QString debtor_balance;  // Balance amount to be paid by debtor

            // Copy results into respective QStrings
            debtor_serial  = query.value(SERIAL).toString();
            debtor_name    = query.value(NAME).toString();
            debtor_amount  = query.value(AMOUNT).toString();
            debtor_paid    = query.value(PAID).toString();
            debtor_balance = QString::number( query.value(AMOUNT).toInt() -
                    query.value(PAID).toInt() );

            // Declare items that will be assigned to the Table Widget
            QTableWidgetItem* serial_item  = new QTableWidgetItem;
            QTableWidgetItem* name_item    = new QTableWidgetItem;
            QTableWidgetItem* amount_item  = new QTableWidgetItem;
            QTableWidgetItem* paid_item    = new QTableWidgetItem;
            QTableWidgetItem* balance_item = new QTableWidgetItem;

            // Set values for items
            serial_item->setText(debtor_serial);
            name_item->setText(debtor_name);
            amount_item->setText(debtor_amount);
            paid_item->setText(debtor_paid);
            balance_item->setText(debtor_balance);

            // Set items for Table Widget and increment the row
            table_widget->setItem( row,   SERIAL,  serial_item );
            table_widget->setItem( row,   NAME,    name_item );
            table_widget->setItem( row,   AMOUNT,  amount_item );
            table_widget->setItem( row,   PAID,    paid_item );
            table_widget->setItem( row++, BALANCE, balance_item );

            // Calculate grand total of amount given to debtors, amount paid by
            // debtors and balance amount to be paid by debtors
            total_amount  += debtor_amount.toInt();
            total_paid    += debtor_paid.toInt();
            total_balance += debtor_balance.toInt();
        }

    // Set Line Edits with their respective values
    total_amount_edit->setText( QString::number(total_amount) );
    total_paid_edit->setText( QString::number(total_paid) );
    total_balance_edit->setText( QString::number(total_balance) );
}
