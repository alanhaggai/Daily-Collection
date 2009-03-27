#include "dailycollectionwindow.h"
#include "createagentdialog.h"
#include "createdebtordialog.h"
#include "editagentdialog.h"
#include "editdebtordialog.h"
#include "edittransactiondialog.h"

DailyCollectionWindow::DailyCollectionWindow(QMainWindow *parent) : QMainWindow(parent) {
    setupUi(this);
    setWindowState(Qt::WindowMaximized);

    connect( action_CreateAgent,       SIGNAL(activated()), this, SLOT(spawnCreateAgent()) );
    connect( action_CreateDebtor,      SIGNAL(activated()), this, SLOT(spawnCreateDebtor()) );
    connect( action_EditAgent,         SIGNAL(activated()), this, SLOT(spawnEditAgent()) );
    connect( action_EditDebtor,        SIGNAL(activated()), this, SLOT(spawnEditDebtor()) );
    connect( action_EditTransaction,   SIGNAL(activated()), this, SLOT(spawnEditTransaction()) );
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
