#ifndef DEBTORTRANSACTIONSDIALOG_H
#define DEBTORTRANSACTIONSDIALOG_H

#include "ui_DebtorTransactionsDialog.h"

class DebtorTransactionsDialog : public QDialog,
        public Ui::DebtorTransactionsDialog
{
    Q_OBJECT

    public slots:
        void SerialEditTextChanged(const QString& debtor_serial);
        void Clear();

    public:
        DebtorTransactionsDialog( QDialog *parent = 0 );
};

#endif
