/*!
 * \file  DaybookBalanceDialog.cpp
 * \brief Displays balance amount to be paid by each debtor under an agent from
 * a certain date
 *
 * \ingroup Daybook
 */

#include "DaybookBalanceDialog.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QProgressDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QProcess>
#include <QSettings>

//! Enumerated constants to represent Table Widget columns
enum {
    SERIAL,
    NAME,
    BALANCE,
};

bool flag_balance = false;

/*!
 * Fetch agent id and agent name from agent table. Populate the values in a map
 * with agent id as key and agent name as value. Set Table Widget column width.
 */
DaybookBalanceDialog::DaybookBalanceDialog(QDialog* parent) :
        QDialog(parent) {
    setupUi(this);

    // Fetch ids and names of agents
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

    // Cycle through the result rows
    while ( query.next() ) {
            int agent_id;        // Agent id
            QString agent_name;  // Agent name

            agent_id   = query.value(0).toInt();
            agent_name = query.value(1).toString();

            // Add agent name to Combo Box and create mapping
            agent_combo->addItem(agent_name);
            agent_map[agent_id] = agent_name;
        }

    agent_combo->setCurrentIndex(-1);  // Clear selection in agent Combo Box

    // Set column widths
    table_widget->setColumnWidth( SERIAL,  50);
    table_widget->setColumnWidth( NAME,    200);
    table_widget->setColumnWidth( BALANCE, 100);

    connect( list_button, SIGNAL( clicked() ), this,
            SLOT( List() ) );
    connect( open_report_in_browser_button, SIGNAL( clicked() ), this,
            SLOT( OpenReportInBrowser() ) );
}

DaybookBalanceDialog::~DaybookBalanceDialog() {
    flag_balance = false;
}

/*!
 * Check if an agent has been selected or not. If not, return. Else, fetch
 * details of debtors, including sum or transaction that has already been paid,
 * on or before the selected date, under the selected agent.
 */
void
DaybookBalanceDialog::List() {

    table_widget->clearContents();
    table_widget->setRowCount(0);

    qint16 agent;
    QString agent_name;
    QString date;

    agent      = agent_map.key( agent_combo->currentText() );
    agent_name = agent_combo->currentText();
    date       = date_calendar->selectedDate().toString(Qt::ISODate);

    if ( -1 == agent_combo->currentIndex() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Warning, "Incomplete Fields",
                "All fields are to be filled.", QMessageBox::Ok );
            msgbox->exec();  // Popup message box to let the user know about it
            return;
        }

    // Add to installation script for creation of `view' in the database
    //      CREATE VIEW debtor_transaction
    //      AS SELECT debtor.serial, debtor.name, debtor.amount,
    //      SUM(transaction.paid) AS paid, (debtor.amount - paid) AS balance,
    //      debtor.agent_id, transaction.date FROM debtor, transaction WHERE
    //      debtor.id = transaction.debtor_id GROUP BY debtor.serial ORDER BY
    //      debtor.serial;

    // Select details of debtors under the selected agent, including their paid
    // amount till the selected date
    QSqlQuery query;
    query.prepare("SELECT debtor.serial, debtor.name, debtor.amount,\
            SUM(transactions.paid) FROM debtor, transactions WHERE\
            debtor.agent_id = :agent_id AND debtor.id = transactions.debtor_id\
            AND transactions.date <= :transaction_date GROUP BY debtor.serial\
            ORDER BY debtor.serial");
    query.bindValue( ":agent_id", agent_map.key(agent_combo->currentText()) );
    query.bindValue( ":transaction_date",
            date_calendar->selectedDate().toString(Qt::ISODate) );

    qint32 count = 0;

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query execution failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    while ( query.next() ) {
        if ( query.value(2).toInt() - query.value(3).toInt() )
            count++;
    }

    query.first();

    // Initialise a Progress Dialog which will be later used for displaying
    // progress in loop
    QProgressDialog progress_dialog( "Retrieving data...", "Abort Fetch", 0,
            count, this );

    // Do not let other dialogs of our application get focus when the Progress
    // Dialog is displayed
    progress_dialog.setWindowModality(Qt::WindowModal);
    progress_dialog.setMinimumDuration(0);  // Minimum duration after which the
    // Progress Dialog is displayed

    // Set number of rows for the Table Widget
    table_widget->setRowCount(count);

    flag_balance = true;

    qint32 row      = 0;  // Represents the current row
    qint32 progress = 0;  // Keeps track of progress

    table_widget->clearContents();  // Clear already listed contents

    QString html = "\
        <html>\n\
            <head>\n\
                <title>Balance Sheet: " + agent_name + " on "
            + date_calendar->selectedDate().toString("dd-MM-yyyy")
            + "</title>\n\
                <style type='text/css'>\n\
                </style>\n\
                <link type='text/css' rel='stylesheet' href='style.css' />\n\
            </head>\n\
            <body>\n\
                <div id='container'>\n\
                    <div id='header'>\n\
                        <h2>Balance Sheet</h2>\n\
                        <table>\n\
                            <tr>\n\
                                <td>Agent</td><td class='right'><b>"
            + agent_name + "</b></td>\n\
                            </tr>\n\
                            <tr>\n\
                                <td>Date</td><td class='right'><b>"
            + date_calendar->selectedDate().toString("dd-MM-yyyy")
            + "</b></td>\n\
                            </tr>\n\
                        </table>\n\
                    </div>\n\
                    <div id='content'>\n\
                        <table>\n\
                            <th>Serial</th><th>Name</th><th>Balance</th>\n\
            ";

    int total_balance = 0;

    do {
            QString debtor_serial;   // Debtor serial number
            QString debtor_name;     // Debtor name
            int     debtor_balance;  // Remaining debts

            debtor_serial  = query.value(SERIAL).toString();
            debtor_name    = query.value(NAME).toString();
            debtor_balance = query.value(2).toInt() - query.value(3).toInt();

            if ( debtor_balance == 0 )
                continue;

            progress_dialog.setValue(progress++);  // Set value for Progress
            //Dialog and increment it
            qApp->processEvents();  // So that we get a responsive interface

            if ( progress_dialog.wasCanceled() ) {
                    // If Progress Dialog's `Abort Fetch' button is clicked, end
                    // further processing by breaking out of the loop
                    break;
                }

            total_balance += debtor_balance;

            // Set items for Table Widget
            QTableWidgetItem* serial_item  = new QTableWidgetItem;
            QTableWidgetItem* name_item    = new QTableWidgetItem;
            QTableWidgetItem* balance_item = new QTableWidgetItem;


            // Set text of Line Edits
            serial_item->setText(debtor_serial);
            name_item->setText(debtor_name);
            balance_item->setText( QString::number(debtor_balance) );

            // Set items for each of the columns
            table_widget->setItem( row,   SERIAL,  serial_item );
            table_widget->setItem( row,   NAME,    name_item );
            table_widget->setItem( row++, BALANCE, balance_item );

            html += "\
                    <tr class='style" + QString::number( row % 2 ) +"'>\n\
                        <td class='right'>" + debtor_serial
                    + "</td><td class='centre'>"
                    + debtor_name + "</td><td class='right'>Rs. "
                    + QString::number(debtor_balance)
                    + "</td>\n\
                    </tr>\n\
                    ";
        } while ( query.next() );

    // Display total balance in total_balance_edit Line Edit
    total_balance_edit->setText( QString::number(total_balance) );

    // Explicitly set maximum value of the Progress Dialog
    progress_dialog.setValue( query.size() );

    html += "\
                            </table>\n\
                            <table>\n\
                                <tr>\n\
                                    <td>Total balance</td><td class='right'><b>Rs. "
            + QString::number(total_balance)
            + "</b></td>\n\
                                </tr>\n\
                            </table>\n\
                        </div>\n\
                    </div>\n\
                </body>\n\
            </html>\n\
            ";

    QFile file("balance.html");

    if ( file.open( QIODevice::WriteOnly | QIODevice::Text ) ) {
            QTextStream out(&file);
            out << html;

            file.close();
        }
}

void
DaybookBalanceDialog::OpenReportInBrowser() {
    if ( flag_balance == false ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Warning, "Data required to produce report",
                "Please list the balance before opening the report.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    QProcess* browser_process = new QProcess;

    QSettings settings;
    if ( settings.value("Browser/preferred").toString() != "" ) {
        browser_process->start( settings.value("Browser/preferred").toString()
                + " " + QDir::currentPath() + QString( QDir::separator() )
                + "balance.html" );
    }
    else {
#ifdef Q_OS_WIN32
        QString file = QDir::currentPath() + "\\balance.html";
        browser_process->start(
            "\"C:\\\\Program Files\\Internet Explorer\\iexplore.exe\" "
            + file );
#else
        QString file = QDir::currentPath() + "/balance.html";
        browser_process->execute( "firefox " + file );
#endif
    }
}
