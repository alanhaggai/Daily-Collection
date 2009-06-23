#ifndef DEBTORINSTALLMENTDIALOG_H
#define DEBTORINSTALLMENTDIALOG_H

#include "ui_DebtorInstallmentDialog.h"

class DebtorInstallmentDialog : public QDialog,
        public Ui::DebtorInstallmentDialog {
        Q_OBJECT

    public slots:
        void PopulateValuesOnSerialChange(const QString& debtor_serial);
        void CalculateAmount(const QString& number_of_installments);
        void FiftyRadioToggled(bool checked);
        void HundredRadioToggled(bool checked);
        void SaveInstallment();

    public:
        DebtorInstallmentDialog( QDialog *parent = 0 );

    private:
        int installment;
};

#endif
