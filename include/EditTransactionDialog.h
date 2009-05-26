#ifndef EDITTRANSACTIONDIALOG_H
#define EDITTRANSACTIONDIALOG_H

#include "ui_EditTransactionDialog.h"

class EditTransactionDialog : public QDialog, public Ui::EditTransactionDialog {
        Q_OBJECT

    public:
        EditTransactionDialog( QDialog *parent = 0 );

    public slots:
        void populateTableWidgetSerialEdit(const QString&);
        void fetchItem(QTableWidgetItem *);
        void clearTransaction();
        void saveTransaction();
        void deleteTransaction();

    private:
        int currentRow, transactionId;
        QString debtorName;
};

#endif
