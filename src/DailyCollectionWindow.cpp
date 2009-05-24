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
#include <QDateTime>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QDir>

#ifdef Q_OS_WIN32
QString mysql     = "C:\\\\MySQL\\bin\\mysql.exe";
QString mysqldump = "C:\\\\MySQL\\bin\\mysqldump.exe";
#else
QString mysql     = "/usr/bin/mysql";
QString mysqldump = "/usr/bin/mysqldump";
#endif

DailyCollectionWindow::DailyCollectionWindow(QMainWindow *parent) :
        QMainWindow(parent) {
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
DailyCollectionWindow::SpawnCreateAgentDialog() {
    CreateAgentDialog* create_agent_dialog  = new CreateAgentDialog;
    mdiArea->addSubWindow(create_agent_dialog);
    create_agent_dialog->show();
}

void
DailyCollectionWindow::SpawnCreateDebtorDialog() {
    CreateDebtorDialog* create_debtor_dialog  = new CreateDebtorDialog;
    mdiArea->addSubWindow(create_debtor_dialog);
    create_debtor_dialog->show();
}

void
DailyCollectionWindow::SpawnEditAgentDialog() {
    EditAgentDialog* edit_agent_dialog  = new EditAgentDialog;
    mdiArea->addSubWindow(edit_agent_dialog);
    edit_agent_dialog->show();
}

void
DailyCollectionWindow::SpawnEditDebtorDialog() {
    EditDebtorDialog* edit_debtor_dialog = new EditDebtorDialog;
    mdiArea->addSubWindow(edit_debtor_dialog);
    edit_debtor_dialog->show();
}

void
DailyCollectionWindow::SpawnEditTransactionDialog() {
    EditTransactionDialog* edit_transaction_dialog = new EditTransactionDialog;
    mdiArea->addSubWindow(edit_transaction_dialog);
    edit_transaction_dialog->show();
}

void
DailyCollectionWindow::SpawnDaybookAgentDialog() {
    DaybookAgentDialog* daybook_agent_dialog = new DaybookAgentDialog;
    mdiArea->addSubWindow(daybook_agent_dialog);
    daybook_agent_dialog->show();
}

void
DailyCollectionWindow::SpawnDaybookAllAgentsDialog() {
    DaybookAllAgentsDialog* daybook_all_agents_dialog = new DaybookAllAgentsDialog;
    mdiArea->addSubWindow(daybook_all_agents_dialog);
    daybook_all_agents_dialog->show();
}

void
DailyCollectionWindow::SpawnDaybookBalanceDialog() {
    DaybookBalanceDialog* daybook_balance_dialog = new DaybookBalanceDialog;
    mdiArea->addSubWindow(daybook_balance_dialog);
    daybook_balance_dialog->show();
}

void
DailyCollectionWindow::SpawnDebtorInstallmentDialog() {
    DebtorInstallmentDialog* debtor_installment_dialog = new DebtorInstallmentDialog;
    mdiArea->addSubWindow(debtor_installment_dialog);
    debtor_installment_dialog->show();
}

void
DailyCollectionWindow::SpawnDebtorDetailsDialog() {
    DebtorDetailsDialog* debtor_details_dialog = new DebtorDetailsDialog;
    mdiArea->addSubWindow(debtor_details_dialog);
    debtor_details_dialog->show();
}

void
DailyCollectionWindow::SpawnDebtorTransactionsDialog() {
    DebtorTransactionsDialog* debtor_transactions_dialog = new DebtorTransactionsDialog;
    mdiArea->addSubWindow(debtor_transactions_dialog);
    debtor_transactions_dialog->show();
}

void
DailyCollectionWindow::SpawnTransactionsDialog() {
    TransactionsDialog* transactions_dialog = new TransactionsDialog;
    mdiArea->addSubWindow(transactions_dialog);
    transactions_dialog->show();
}

void
DailyCollectionWindow::Backup() {
    QString suggested_filename = "./backups/" + QDateTime::currentDateTime().toString()
            + ".db";
    QString filename = QFileDialog::getSaveFileName( this, "Backup Database",
            suggested_filename, "Database Files (*.db)" );

    QString database = DbConnect::database;
    QString username = DbConnect::username;
    QString password = DbConnect::password;

    QProcess* mysqldump_process = new QProcess;
    mysqldump_process->setStandardOutputFile( filename, QIODevice::WriteOnly );
    mysqldump_process->start( mysqldump, QStringList() << database << "-u"
            + username << "-p" + password );

    if ( !mysqldump_process->waitForFinished() )
        exit(0);
}

void
DailyCollectionWindow::AutoBackup() {
    DbConnect::Disconnect();

    QString database = DbConnect::database;
    QString username = DbConnect::username;
    QString password = DbConnect::password;

    QDir dir("backups/auto");

    if ( !dir.exists() ) {
            if ( !dir.mkpath(".") ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Warning, "Automatic Backup Failed",
                        "Database has not been automatically backed up.",
                        QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }
        }

    QString filename = dir.absolutePath() + "/"
            + QDateTime::currentDateTime().toString() + ".db";
    QProcess* mysqldump_process = new QProcess;
    mysqldump_process->setStandardOutputFile( filename, QIODevice::WriteOnly );
    mysqldump_process->start( mysqldump, QStringList() << database << "-u"
            + username << "-p" + password );

    if ( !mysqldump_process->waitForFinished() )
        exit(0);
}

void
DailyCollectionWindow::Restore() {
    QString filename = QFileDialog::getOpenFileName( this, "Restore Database",
            ".", "Database Files (*.db)" );

    QString database = DbConnect::database;
    QString username = DbConnect::username;
    QString password = DbConnect::password;

    QString command;

    if ( DbConnect::Connect() ) {
            QProcess* mysql_process = new QProcess;
            mysql_process->setStandardInputFile(filename);
            mysql_process->start( mysql, QStringList() << "-u" + username << "-p"
                    + password );

            if ( !mysql_process->waitForFinished() )
                exit(0);

            if ( mysql_process->exitCode() == 1 ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Information, "Restoration Successful",
                        "Database has been restored successfully.",
                        QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }
        }

    QMessageBox* msgbox = new QMessageBox(
        QMessageBox::Critical, "Restoration Failure",
        "Database restoration has failed.",
        QMessageBox::Ok );
    msgbox->exec();
}
