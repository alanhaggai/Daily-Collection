/*!
 * \file  DailyCollectionWindow.h
 * \brief Declarations for DailyCollectionWindow.cpp
 *
 * \ingroup Main
 */
#ifndef DAILYCOLLECTIONWINDOW_H
#define DAILYCOLLECTIONWINDOW_H

#include "ui_DailyCollectionWindow.h"

class DailyCollectionWindow : public QMainWindow, public Ui::DailyCollectionWindow {
        Q_OBJECT  // so that `moc` gets invoked

    public:
        //! Constructor
        DailyCollectionWindow( QMainWindow *parent = 0 );

    public slots:
        //! Spawns a dialog for creating an agent
        void SpawnCreateAgentDialog();

        //! Spawns a dialog for creating a debtor
        void SpawnCreateDebtorDialog();

        //! Spawns a dialog for editing an agent
        void SpawnEditAgentDialog();

        //! Spawns a dialog for editing a debtor
        void SpawnEditDebtorDialog();

        //! Spawns a dialog for editing a transaction
        void SpawnEditTransactionDialog();

        //! Spawns a dialog that displays daybook of an agent
        void SpawnDaybookAgentDialog();

        //! Spawns a dialog that displays daybook of all agents
        void SpawnDaybookAllAgentsDialog();

        //! Spawns a dialog that displays daybook balance
        void SpawnDaybookBalanceDialog();

        //! Spawns a dialog for making debtor installments
        void SpawnDebtorInstallmentDialog();

        //! Spawns a dialog that displays details of debtors
        void SpawnDebtorDetailsDialog();

        //! Spawns a dialog that displays transaction details of debtors
        void SpawnDebtorTransactionsDialog();

        //! Spawns a dialog that displays transactions by debtors under an agent
        void SpawnTransactionsDialog();

        void Backup();
        void AutoBackup();
        void Restore();
};

#endif
