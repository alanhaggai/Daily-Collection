/*!
 * \file  CreateDebtorDialog.h
 * \brief Declarations for CreateDebtorDialog.cpp
 *
 * \ingroup Create
 */

#ifndef CREATEDEBTORDIALOG_H
#define CREATEDEBTORDIALOG_H

#include "ui_CreateDebtorDialog.h"

//! Declares members and slots for CreateDebtorDialog.cpp
class CreateDebtorDialog : public QDialog, public Ui::CreateDebtorDialog {
        Q_OBJECT  // so that `moc` gets invoked

    public:
        //! Constructor
        CreateDebtorDialog( QDialog *parent = 0 );

    public slots:
        //! Fetch input data of agent and save to database.
        void SaveDebtor();

    private:
        //! Stores agent id as key and agent name as value.
        QMap<int, QString> agent_map;
};

#endif
