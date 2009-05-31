/*!
 * \file  DaybookBalanceDialog.h
 * \brief Declarations for DaybookBalanceDialog.cpp
 *
 * \ingroup Daybook
 */

#ifndef DAYBOOKBALANCEDIALOG_H
#define DAYBOOKBALANCEDIALOG_H

#include "ui_DaybookBalanceDialog.h"

//! Declares members, slot and map for DaybookBalance.cpp
class DaybookBalanceDialog : public QDialog, public Ui::DaybookBalanceDialog {
        Q_OBJECT  // Invoke `moc` for generation of slot code

    public:
        //! Constructor
        DaybookBalanceDialog( QDialog *parent = 0 );
        ~DaybookBalanceDialog();

    public slots:
        //! \brief Calculate balance of each debtor under an agent and display
        void List();
        void OpenReportInBrowser();

    private:
        /*!
         * Map; with agent id as key and agent name as value
         * \param int     Agent id
         * \param QString Agent name
         */
        QMap<int, QString> agent_map;
};

#endif
