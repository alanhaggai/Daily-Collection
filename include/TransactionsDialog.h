#ifndef TRANSACTIONSDIALOG_H
#define TRANSACTIONSDIALOG_H

#include "ui_TransactionsDialog.h"

class TransactionsDialog : public QDialog,
        public Ui::TransactionsDialog {
        Q_OBJECT

    public slots:
        void ListTransactions();
        void OpenReportInBrowser();

    public:
        TransactionsDialog( QDialog *parent = 0 );

    private:
        QMap<int, QString> agent_map;
};

#endif
