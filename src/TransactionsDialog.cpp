/*!
 * \file  TransactionsDialog.cpp
 * \brief Lists transactions made by debtors under an agent
 *
 * \ingroup Main
 */

#include "TransactionsDialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QDateTime>

#include <iostream>
#include <fstream>
#include <stdlib.h>

enum
{
    SERIAL,
    NAME,
    PAID,
};

//! Constructor
TransactionsDialog::TransactionsDialog(QDialog* parent) :
        QDialog( parent, Qt::Tool )
{
    setupUi(this);  // Setup UI

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

    table_widget->setColumnWidth( SERIAL, 50 );
    table_widget->setColumnWidth( NAME,   200 );
    table_widget->setColumnWidth( PAID,   80 );

    connect( list_transactions_button, SIGNAL( clicked() ), this,
            SLOT( ListTransactions() ) );
    connect( open_report_in_browser_button, SIGNAL( clicked() ), this,
            SLOT( OpenReportInBrowser() ) );
}

void
TransactionsDialog::ListTransactions()
{
    table_widget->clearContents();
    table_widget->setRowCount(0);

    qint16 agent;
    QString agent_name;
    QString from_date;
    QString to_date;

    agent      = agent_map.key( agent_combo->currentText() );
    agent_name = agent_combo->currentText();
    to_date    = to_calendar->selectedDate().toString(Qt::ISODate);
    from_date  = from_calendar->selectedDate().toString(Qt::ISODate);


    if ( -1 == agent_combo->currentIndex() )
    {
        QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Information, "Incomplete Fields",
                "All fields are to be filled.", QMessageBox::Ok );
        msgbox->show();  // Popup message box to let the user know about it
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT debtor.serial, debtor.name, sum(transaction.paid)\
            FROM debtor, agent, transaction WHERE transaction.agent_id\
            = agent.id AND debtor.id = transaction.debtor_id AND\
            transaction.date >= :from_date AND transaction.date <= :to_date AND\
            agent.id = :agent_id GROUP BY debtor.serial ORDER BY debtor.serial\
            ASC");
    query.bindValue( ":from_date", from_date );
    query.bindValue( ":to_date", to_date );
    query.bindValue( ":agent_id", agent );

    if ( !query.exec() )
    {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }

    table_widget->setRowCount( query.size() );

    qint16 row   = 0;
    qint32 total = 0;

    QString html = "\
        <html>\n\
            <head>\n\
                <title>Transactions by " + agent_name + " from " + from_calendar->selectedDate().toString("dd-MM-yyyy") + " to " + to_calendar->selectedDate().toString("dd-MM-yyyy") + "</title>\n\
                <style type='text/css'>\n\
                </style>\n\
                <link type='text/css' rel='stylesheet' href='style.css' />\n\
            </head>\n\
            <body>\n\
                <div id='container'>\n\
                    <div id='header'>\n\
                        <h2>Transactions</h2>\n\
                        <table>\n\
                            <tr>\n\
                                <td>Agent</td><td class='right'><b>" + agent_name + "</b></td>\n\
                            </tr>\n\
                            <tr>\n\
                                <td>From</td><td class='right'><b>" + from_calendar->selectedDate().toString("dd-MM-yyyy") + "</b></td>\n\
                            </tr>\n\
                            <tr>\n\
                                <td>To</td><td class='right'><b>" + to_calendar->selectedDate().toString("dd-MM-yyyy") + "</b></td>\n\
                            </tr>\n\
                        </table>\n\
                    </div>\n\
                    <div id='content'>\n\
                        <table>\n\
                            <th>Serial</th><th>Name</th><th>Transaction</th>\n\
            ";

    while ( query.next() )
    {
        QString serial;
        QString name;
        QString paid;

        serial = query.value(SERIAL).toString();
        name   = query.value(NAME).toString();
        paid   = query.value(PAID).toString();

        QTableWidgetItem* serial_item = new QTableWidgetItem;
        QTableWidgetItem* name_item   = new QTableWidgetItem;
        QTableWidgetItem* paid_item   = new QTableWidgetItem;

        serial_item->setText(serial);
        name_item->setText(name);
        paid_item->setText(paid);

        total += paid.toInt();

        table_widget->setItem( row,   SERIAL, serial_item );
        table_widget->setItem( row,   NAME,   name_item );
        table_widget->setItem( row++, PAID,   paid_item );

        html += "\
                    <tr style='$style'>\n\
                        <td class='right'>" + serial + "</td><td class='centre'>" + name + "</td><td class='right'>Rs. " + paid + "</td>\n\
                    </tr>\n\
                    ";
    }

    html += "\
                            </table>\n\
                            <table>\n\
                                <tr>\n\
                                    <td>Total</td><td class='right'><b>Rs. " + QString::number(total) + "</b></td>\n\
                                </tr>\n\
                            </table>\n\
                        </div>\n\
                    </div>\n\
                </body>\n\
            </html>\n\
            ";

    total_edit->setText( QString::number(total) );

    {
        using namespace std;

        ofstream outfile( "transactions.html", ios::out );
        if ( outfile.is_open() )
        {
            outfile << html.toStdString();
            outfile.close();
        }
    }
}

void
TransactionsDialog::OpenReportInBrowser()
{
    ListTransactions();

    QString file = QDir::toNativeSeparators( QDir::currentPath() +
            "/transactions.html" );
    system( "firefox " + file.toAscii() );
}
