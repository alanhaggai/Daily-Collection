/*!
 * \file  DaybookAllAgentsDialog.h
 * \brief Declarations for DaybookAllAgentsDialog.cpp
 *
 * \ingroup Daybook
 */

#ifndef DAYBOOKALLAGENTSDIALOG_H
#define DAYBOOKALLAGENTSDIALOG_H

#include "ui_DaybookAllAgentsDialog.h"

//! Declares members and function for DaybookAllAgentsDialog.cpp
class DaybookAllAgentsDialog : public QDialog, public Ui::DaybookAllAgentsDialog
{
    public:
        //! Constructor
        DaybookAllAgentsDialog( QDialog *parent = 0 );

        /*!
         * Calculate total amount given and total amount remitted by each agent
         * and display
         */
        void PopulateTableWidget();
};

#endif
