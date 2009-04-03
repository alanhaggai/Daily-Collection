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

DailyCollectionWindow::DailyCollectionWindow(QMainWindow *parent) : QMainWindow(parent) {
    setupUi(this);
    setWindowState(Qt::WindowMaximized);

    connect( action_CreateAgent,       SIGNAL(activated()), this, SLOT(spawnCreateAgent()) );
    connect( action_CreateDebtor,      SIGNAL(activated()), this, SLOT(spawnCreateDebtor()) );
    connect( action_EditAgent,         SIGNAL(activated()), this, SLOT(spawnEditAgent()) );
    connect( action_EditDebtor,        SIGNAL(activated()), this, SLOT(spawnEditDebtor()) );
    connect( action_EditTransaction,   SIGNAL(activated()), this, SLOT(spawnEditTransaction()) );
    connect( action_DaybookAgent,      SIGNAL(activated()), this, SLOT(spawnDaybookAgent()) );
    connect( action_DaybookAllAgents,  SIGNAL(activated()), this, SLOT(spawnDaybookAllAgents()) );
    connect( action_DaybookBalance,    SIGNAL(activated()), this, SLOT(spawnDaybookBalance()) );
    connect( action_DebtorInstallment, SIGNAL(activated()), this, SLOT(spawnDebtorInstallment()) );
}

void DailyCollectionWindow::spawnCreateAgent() {
    CreateAgentDialog *createAgentDialog  = new CreateAgentDialog;
    createAgentDialog->show();
}

void DailyCollectionWindow::spawnCreateDebtor() {
    CreateDebtorDialog *createDebtorDialog  = new CreateDebtorDialog;
    createDebtorDialog->show();
}

void DailyCollectionWindow::spawnEditAgent() {
    EditAgentDialog *editAgentDialog  = new EditAgentDialog;
    editAgentDialog->show();
}

void DailyCollectionWindow::spawnEditDebtor() {
    EditDebtorDialog *editDebtorDialog = new EditDebtorDialog;
    editDebtorDialog->show();
}

void DailyCollectionWindow::spawnEditTransaction() {
    EditTransactionDialog *editTransactionDialog = new EditTransactionDialog;
    editTransactionDialog->show();
}

void DailyCollectionWindow::spawnDaybookAgent() {
    DaybookAgentDialog *daybookAgentDialog = new DaybookAgentDialog;
    daybookAgentDialog->show();
}

void DailyCollectionWindow::spawnDaybookAllAgents() {
    DaybookAllAgentsDialog *daybookAllAgentsDialog = new DaybookAllAgentsDialog;
    daybookAllAgentsDialog->show();
}

void DailyCollectionWindow::spawnDaybookBalance() {
    DaybookBalanceDialog *daybookBalanceDialog = new DaybookBalanceDialog;
    daybookBalanceDialog->show();
}

void DailyCollectionWindow::spawnDebtorInstallment() {
    DebtorInstallmentDialog *debtorInstallmentDialog = new DebtorInstallmentDialog;
    debtorInstallmentDialog->show();
}
