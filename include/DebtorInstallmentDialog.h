#ifndef DEBTORINSTALLMENTDIALOG_H
#define DEBTORINSTALLMENTDIALOG_H

#include "ui_DebtorInstallmentDialog.h"

class DebtorInstallmentDialog : public QDialog, public Ui::DebtorInstallmentDialog {
    public:
        DebtorInstallmentDialog( QDialog *parent = 0 );
};

#endif
