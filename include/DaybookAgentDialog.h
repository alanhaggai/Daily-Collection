/*!
 * \file  DaybookAgentDialog.h
 * \brief Declarations for DaybookAgentDialog.cpp
 *
 * \ingroup Daybook
 */

#ifndef DAYBOOKAGENTDIALOG_H
#define DAYBOOKAGENTDIALOG_H

#include "ui_DaybookAgentDialog.h"

//! Declares members, slot and map for DaybookAgentDialog.cpp
class DaybookAgentDialog : public QDialog, public Ui::DaybookAgentDialog {
    Q_OBJECT  // so that `moc` gets invoked

    public:
        //! Constructor
        DaybookAgentDialog( QDialog *parent = 0 );

    public slots:
        /*!
         * \param current_index Current index of agent Combo Box
         * \brief Calculate amount details and populate Table Widget
         */
        void PopulateTableWidget(int);

    private:
        /*!
         * Map; with agent id as key and agent name as value
         * \param int     Agent id
         * \param QString Agent name
         */
        QMap<int, QString> agent_map;
};

#endif
