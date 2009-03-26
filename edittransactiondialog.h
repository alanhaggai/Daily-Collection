#ifndef EDITTRANSACTIONDIALOG_H
#define EDITTRANSACTIONDIALOG_H

#include "ui_edittransactiondialog.h"

class EditTransactionDialog : public QDialog, public Ui::EditTransactionDialog {
    Q_OBJECT

    public:
        EditTransactionDialog( QDialog *parent = 0 );

    public slots:
        void populateTableWidgetSerialEdit(const QString&);
        void fetchItem(QTableWidgetItem *);
        void clearTransaction();

    private:
        int currentRow, debtorId;
        QString debtorName;
};

#endif
