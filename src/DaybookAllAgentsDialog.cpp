/*!
 * \file  DaybookAllAgentsDialog.cpp
 * \brief Displays total amount given and remitted by each agent
 *
 * \ingroup Daybook
 */

#include "DaybookAllAgentsDialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

//! Represent individual columns in Table Widget
enum
{
    NAME,
    AMOUNT_GIVEN,
    AMOUNT_REMITTED,
    BALANCE,
};

//! Constructor
DaybookAllAgentsDialog::DaybookAllAgentsDialog(QDialog* parent) :
    QDialog( parent, Qt::Tool )
{
    setupUi(this);

    // Populate Table Widget in the moment the dialog is created
    PopulateTableWidget();
}

/*!
 * Fetch ids and names of agents from agent table. Calculate total amount given
 * and total amount remitted. Populate Table Widget with items.
 */
void
DaybookAllAgentsDialog::PopulateTableWidget()
{
    // Set column width
    table_widget->setColumnWidth( NAME,            200);
    table_widget->setColumnWidth( AMOUNT_GIVEN,    150);
    table_widget->setColumnWidth( AMOUNT_REMITTED, 150);
    table_widget->setColumnWidth( BALANCE,         150);

    // Prepare query to select id and name from agent table
    QSqlQuery agent_query, amount_query, transaction_query;
    agent_query.prepare("SELECT id, name FROM agent");

    if ( !agent_query.exec() )
    {
        // Query failed to execute due to an error
        qDebug() << agent_query.lastError();
        qFatal("Failed to execute query.");
    }

    // Set the number of rows for Table Widget by using the query result size
    table_widget->setRowCount( agent_query.size() );
    
    int row = 0;  // Row counter

    // Loop through the rows, providing a new agent each time
    while ( agent_query.next() )
    {
        QString agent_id   = agent_query.value(0).toString();
        QString agent_name = agent_query.value(1).toString();

        // Sum all debtors' amounts with regard to each agent
        amount_query.prepare("SELECT SUM(amount) FROM debtor\
            WHERE agent_id = :agent_id");
        amount_query.bindValue( ":agent_id", agent_id );
        if ( !amount_query.exec() )
        {
            // Query execution failed
            qDebug() << amount_query.lastError();
            qFatal("Failed to execute query.");
            return;
        }

        amount_query.next();  // Start cycling amount query results
        // Store the amount given to each debtor
        int agent_amount_given = amount_query.value(0).toInt();

        // Sum paid column of each debtor whose agent is the current agent
        transaction_query.prepare("SELECT SUM(paid) FROM transaction\
            WHERE agent_id = :agent_id");
        transaction_query.bindValue( ":agent_id", agent_id );
        if ( !transaction_query.exec() )
        {
            // Query execution failed
            qDebug() << transaction_query.lastError();
            qFatal("Failed to execute query.");
        }

        transaction_query.next();  // Cycle through the results
        int agent_amount_remitted = transaction_query.value(0).toInt();
        int agent_balance         = agent_amount_given - agent_amount_remitted;

         // Output debug information
        qDebug() << "DaybookAllAgentsDialog::PopulateTableWidget() - "
            << "Name: " << agent_name;
        qDebug() << "DaybookAllAgentsDialog::PopulateTableWidget() - "
            << "Amount Given: " << agent_amount_given;
        qDebug() << "DaybookAllAgentsDialog::PopulateTableWidget() - "
            << "Amount Remitted: " << agent_amount_remitted;
        qDebug() << "DaybookAllAgentsDialog::PopulateTableWidget() - "
            << "Balance: " << agent_balance;

       // Create Table Widget items
        QTableWidgetItem* name_item            = new QTableWidgetItem;
        QTableWidgetItem* amount_given_item    = new QTableWidgetItem;
        QTableWidgetItem* amount_remitted_item = new QTableWidgetItem;
        QTableWidgetItem* balance_item         = new QTableWidgetItem;

        // Assign values to the items
        name_item->setText(agent_name);
        amount_given_item->setText( QString::number(agent_amount_given) );
        amount_remitted_item->setText( QString::number(agent_amount_remitted) );
        balance_item->setText( QString::number(agent_balance) );

        // Assign an item to each column of Table Widget, and then increment
        // row counter
        table_widget->setItem( row,   NAME,            name_item );
        table_widget->setItem( row,   AMOUNT_GIVEN,    amount_given_item );
        table_widget->setItem( row,   AMOUNT_REMITTED, amount_remitted_item );
        table_widget->setItem( row++, BALANCE,         balance_item );
    }
}
