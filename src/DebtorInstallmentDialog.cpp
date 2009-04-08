#include "DebtorInstallmentDialog.h"

/*
 * Construct the dialog as a tool window rather than a main window.
 */
DebtorInstallmentDialog::DebtorInstallmentDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
    setupUi(this);  // setup UI
}

/*
 * This function executes when the serial number is entered/changed. It fetches
 * the debtor's name, which corresponds to the serial number, and selects the 
 * proper installment.
 */
/*
void
DebtorInstallmentDialog::PopulateValuesOnSerialChange(const QString& serial) {
}
*/
