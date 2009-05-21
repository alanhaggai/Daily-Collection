/*!
 * \file  DailyCollectionWindow.cpp
 * \brief Main window
 *
 * \ingroup Main
 */

// Include all class declarations
#include "DailyCollectionWindow.h"
#include "CreateAgentDialog.h"
#include "CreateDebtorDialog.h"
#include "EditAgentDialog.h"
#include "EditDebtorDialog.h"
#include "EditTransactionDialog.h"
#include "DaybookAgentDialog.h"
#include "DaybookAllAgentsDialog.h"
#include "DaybookBalanceDialog.h"
#include "DebtorInstallmentDialog.h"
#include "DebtorDetailsDialog.h"
#include "DebtorTransactionsDialog.h"
#include "TransactionsDialog.h"
#include "DbConnect.h"

#include <QFileDialog>
#include <QSettings>
#include <QDateTime>

DailyCollectionWindow::DailyCollectionWindow(QMainWindow *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    setWindowState(Qt::WindowMaximized);

    // Connect menu actions to slots that spawn respective dialogs
    connect( action_CreateAgent,
        SIGNAL( activated() ), this, SLOT( SpawnCreateAgentDialog() ) );
    connect( action_CreateDebtor,
        SIGNAL( activated() ), this, SLOT( SpawnCreateDebtorDialog() ) );
    connect( action_EditAgent,
        SIGNAL( activated() ), this, SLOT( SpawnEditAgentDialog() ) );
    connect( action_EditDebtor,
        SIGNAL( activated() ), this, SLOT( SpawnEditDebtorDialog() ) );
    connect( action_EditTransaction,
        SIGNAL( activated() ), this, SLOT( SpawnEditTransactionDialog() ) );
    connect( action_DaybookAgent,
        SIGNAL( activated() ), this, SLOT( SpawnDaybookAgentDialog() ) );
    connect( action_DaybookAllAgents,
        SIGNAL( activated() ), this, SLOT( SpawnDaybookAllAgentsDialog() ) );
    connect( action_DaybookBalance,
        SIGNAL( activated() ), this, SLOT( SpawnDaybookBalanceDialog() ) );
    connect( action_DebtorInstallment,
        SIGNAL( activated() ), this, SLOT( SpawnDebtorInstallmentDialog() ) );
    connect( action_DebtorDetails,
        SIGNAL( activated() ), this, SLOT( SpawnDebtorDetailsDialog() ) );
    connect( action_DebtorTransactions,
        SIGNAL( activated() ), this, SLOT( SpawnDebtorTransactionsDialog() ) );
    connect( action_Transactions,
        SIGNAL( activated() ), this, SLOT( SpawnTransactionsDialog() ) );
    connect( action_Backup,
        SIGNAL( activated() ), this, SLOT( Backup() ) );
    connect( action_Restore,
        SIGNAL( activated() ), this, SLOT( Restore() ) );
    connect( qApp, SIGNAL( aboutToQuit() ), this, SLOT( AutoBackup() ) );
}

// Spawn{,,,,,} all those dialogs

void
DailyCollectionWindow::SpawnCreateAgentDialog()
{
    CreateAgentDialog* create_agent_dialog  = new CreateAgentDialog;
    create_agent_dialog->show();
}

void
DailyCollectionWindow::SpawnCreateDebtorDialog()
{
    CreateDebtorDialog* create_debtor_dialog  = new CreateDebtorDialog;
    create_debtor_dialog->show();
}

void
DailyCollectionWindow::SpawnEditAgentDialog()
{
    EditAgentDialog* edit_agent_dialog  = new EditAgentDialog;
    edit_agent_dialog->show();
}

void
DailyCollectionWindow::SpawnEditDebtorDialog()
{
    EditDebtorDialog* edit_debtor_dialog = new EditDebtorDialog;
    edit_debtor_dialog->show();
}

void
DailyCollectionWindow::SpawnEditTransactionDialog()
{
    EditTransactionDialog* edit_transaction_dialog = new EditTransactionDialog;
    edit_transaction_dialog->show();
}

void
DailyCollectionWindow::SpawnDaybookAgentDialog()
{
    DaybookAgentDialog* daybook_agent_dialog = new DaybookAgentDialog;
    daybook_agent_dialog->show();
}

void
DailyCollectionWindow::SpawnDaybookAllAgentsDialog()
{
    DaybookAllAgentsDialog* daybook_all_agents_dialog = new DaybookAllAgentsDialog;
    daybook_all_agents_dialog->show();
}

void
DailyCollectionWindow::SpawnDaybookBalanceDialog()
{
    DaybookBalanceDialog* daybook_balance_dialog = new DaybookBalanceDialog;
    daybook_balance_dialog->show();
}

void
DailyCollectionWindow::SpawnDebtorInstallmentDialog()
{
    DebtorInstallmentDialog* debtor_installment_dialog = new DebtorInstallmentDialog;
    debtor_installment_dialog->show();
}

void
DailyCollectionWindow::SpawnDebtorDetailsDialog()
{
    DebtorDetailsDialog* debtor_details_dialog = new DebtorDetailsDialog;
    debtor_details_dialog->show();
}

void
DailyCollectionWindow::SpawnDebtorTransactionsDialog()
{
    DebtorTransactionsDialog* debtor_transactions_dialog = new DebtorTransactionsDialog;
    debtor_transactions_dialog->show();
}

void
DailyCollectionWindow::SpawnTransactionsDialog()
{
    TransactionsDialog* transactions_dialog = new TransactionsDialog;
    transactions_dialog->show();
}

void
DailyCollectionWindow::Backup()
{
    QString suggested_filename = "./backups/" + QDateTime::currentDateTime().toString()
            + ".db";
    QString filename = QFileDialog::getSaveFileName( this, "Backup Database",
            suggested_filename, "Database Files (*.db)" );
    //system( "\"C:\\Program Files\\MySQL\\MySQL Server 5.1\\bin\\mysqldump.exe\" daily_collection -uroot -pcubegin" )

    QSettings settings;
    QString mysqldump = settings.value("MySQL/mysqldump").toString();
    QString database  = settings.value("MySQL/database").toString();
    QString username  = settings.value("MySQL/username").toString();
    QString password  = settings.value("MySQL/password").toString();

    QString command = "\"" + mysqldump + "\" " + database + " -u" + username
            + " -p" + password + " > " + "\"" + filename + "\"";
    system( command.toAscii() );
}

void
DailyCollectionWindow::AutoBackup()
{
    QSettings settings;
    QString mysqldump = settings.value("MySQL/mysqldump").toString();
    QString database  = settings.value("MySQL/database").toString();
    QString username  = settings.value("MySQL/username").toString();
    QString password  = settings.value("MySQL/password").toString();

#ifdef __WIN32
    system("mkdir .\\backups\\auto");
    QString filename = ".\\backups\\auto\\" + QDateTime::currentDateTime().toString()
            + ".db";
#elif __LINUX
    system("mkdir -p ./backups/auto");
    QString filename = "./backups/auto/" + QDateTime::currentDateTime().toString()
            + ".db";
#endif

    QString command = "\"" + mysqldump + "\" " + database + " -u" + username
            + " -p" + password + " > " + "\"" + filename + "\"";
    system( command.toAscii() );
}

void
DailyCollectionWindow::Restore()
{
    QString filename = QFileDialog::getOpenFileName( this, "Restore Database",
            ".", "Database Files (*.db)" );

    QSettings settings;
    QString mysql     = settings.value("MySQL/mysql").toString();
    QString database  = settings.value("MySQL/database").toString();
    QString username  = settings.value("MySQL/username").toString();
    QString password  = settings.value("MySQL/password").toString();

    QString command;

    if ( !DbConnect() )
    {
        QString execute = "CREATE DATABASE " + database;
        command = "\"" + mysql + "\" -u" + username + " -p" + password
                + " -e \"" + execute + "\"";
        system( command.toAscii() );

        command = "\"" + mysql + "\" " + database + " -u" + username
                + " -p" + password + " < " + "\"" + filename + "\"";
        system( command.toAscii() );

        execute = "ALTER TABLE debtor DROP paid";
        command = "\"" + mysql + "\" " + database + " -u" + username
                + " -p" + password + " -e \"" + execute + "\"";
        system( command.toAscii() );
        qDebug() << command;

        execute = "CREATE VIEW debtor_transaction AS SELECT debtor.serial,\
               debtor.name, debtor.amount, SUM(transaction.paid) AS paid,\
               (debtor.amount - paid) AS balance, debtor.agent_id,\
               transaction.date FROM debtor, transaction WHERE debtor.id\
               = transaction.debtor_id GROUP BY debtor.serial ORDER BY\
               debtor.serial;";
        command = "\"" + mysql + "\" " + database + " -u" + username + " -p"
                + password + " -e \"" + execute + "\"";
        system( command.toAscii() );
        qDebug() << command;

        DbConnect();
    }
    else {
        command = "\"" + mysql + "\" " + database + " -u" + username
                + " -p" + password + " < " + "\"" + filename + "\"";
        system( command.toAscii() );
    }
}
