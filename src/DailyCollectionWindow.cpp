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
#include <QStringList>
#include <QFile>
#include <QSettings>

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
            SIGNAL( activated() ), this,
            SLOT( SpawnEditTransactionDialog() ) );
    connect( action_DaybookAgent,
            SIGNAL( activated() ), this,
            SLOT( SpawnDaybookAgentDialog() ) );
    connect( action_DaybookAllAgents,
            SIGNAL( activated() ), this,
            SLOT( SpawnDaybookAllAgentsDialog() ) );
    connect( action_DaybookBalance,
            SIGNAL( activated() ), this, SLOT( SpawnDaybookBalanceDialog() ) );
    connect( action_DebtorInstallment,
            SIGNAL( activated() ), this,
            SLOT( SpawnDebtorInstallmentDialog() ) );
    connect( action_DebtorDetails,
            SIGNAL( activated() ), this, SLOT( SpawnDebtorDetailsDialog() ) );
    connect( action_DebtorTransactions,
            SIGNAL( activated() ), this,
            SLOT( SpawnDebtorTransactionsDialog() ) );
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
    DaybookAllAgentsDialog* daybook_all_agents_dialog = new
    DaybookAllAgentsDialog;
    mdiArea->addSubWindow(daybook_all_agents_dialog);
    daybook_all_agents_dialog->show();
}

void
DailyCollectionWindow::SpawnDaybookBalanceDialog() {
    DaybookBalanceDialog* daybook_balance_dialog = new
    DaybookBalanceDialog;
    mdiArea->addSubWindow(daybook_balance_dialog);
    daybook_balance_dialog->show();
}

void
DailyCollectionWindow::SpawnDebtorInstallmentDialog() {
    DebtorInstallmentDialog* debtor_installment_dialog = new
    DebtorInstallmentDialog;
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
    DebtorTransactionsDialog* debtor_transactions_dialog = new
    DebtorTransactionsDialog;
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
    QString suggested_filename = QDir::toNativeSeparators(
            QCoreApplication::applicationDirPath() + "/backups/"
            + QDateTime::currentDateTime().toString("MMM d yyyy hh.mm.ss") );
    QString filename = QFileDialog::getSaveFileName( this, "Backup database",
            suggested_filename, "Database Files (*.db)" );

    if ( filename.isEmpty() )
        return;

    QFile backup_file;

    if ( backup_file.exists(filename) )
        backup_file.remove(filename);

    if ( !backup_file.copy( "daily_collection.db", filename ) ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Warning, "Backup failed",
                "Database has not been backed up.",
                QMessageBox::Ok );
            msgbox->exec();
        }
    else {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Warning, "Backup successful",
                "Database has been backed up successfully.",
                QMessageBox::Ok );
            msgbox->exec();
        }
}

void
DailyCollectionWindow::AutoBackup() {
    QSettings settings( "daily_collection.ini", QSettings::IniFormat );

    if ( settings.value("auto-backup").toBool() != false ) {
        QFile backup_file;

        if ( !backup_file.copy( "daily_collection.db",
                QDir::toNativeSeparators( QCoreApplication::applicationDirPath()
                + "/backups/auto/")
                + QDateTime::currentDateTime().toString("MMM d yyyy hh.mm.ss")
                + ".db" ) ) {
                QMessageBox* msgbox = new QMessageBox(
                    QMessageBox::Warning, "Automatic backup failed",
                    "Database has not been backed up automatically.",
                    QMessageBox::Ok );
                msgbox->exec();
            }
    }
}

void
DailyCollectionWindow::Restore() {
    QString filename = QFileDialog::getOpenFileName( this, "Restore database",
            QCoreApplication::applicationDirPath() , "Database Files (*.db)" );

    if ( filename.isEmpty() )
        return;

    AutoBackup();

    DbConnect::Disconnect();

    QFile restore_file;
    restore_file.remove("daily_collection.db");

    if ( restore_file.copy( filename, "daily_collection.db" ) ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Information, "Restore successful",
                "Database has been restored successfully.",
                QMessageBox::Ok );
            msgbox->exec();
        }
    else {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Restore failed",
                "Database has not been restored.",
                QMessageBox::Ok );
            msgbox->exec();
        }

    DbConnect::Connect();
}
