/*!
 * \file  DebtorDetailsDialog.h
 * \brief Declarations for DebtorDetailsDialog.cpp
 *
 * \ingroup Debtor
 */

#ifndef DEBTORDETAILSDIALOG_H
#define DEBTORDETAILSDIALOG_H

#include <QSqlQuery>

#include "ui_DebtorDetailsDialog.h"

//! Declares members and slot for DebtorDetailsDialog.cpp
class DebtorDetailsDialog : public QDialog, public Ui::DebtorDetailsDialog {
    Q_OBJECT  // so that `moc` gets invoked

    public:
        //! Constructor
        DebtorDetailsDialog( QDialog *parent = 0 );

    public slots:
        void SerialEditTextChanged(QString);
        void NameEditTextChanged(QString);
};

#endif
