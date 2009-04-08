/*!
 * \file  CreateAgentDialog.h
 * \brief Declarations for CreateAgentDialog.cpp
 *
 * \ingroup Create
 */

#ifndef CREATEAGENTDIALOG_H
#define CREATEAGENTDIALOG_H

#include "ui_CreateAgentDialog.h"

//! Declares members and slots for CreateAgentDialog.cpp
class CreateAgentDialog : public QDialog, public Ui::CreateAgentDialog {
    Q_OBJECT  // so that `moc` gets invoked

    public:
        //! Constructor
        CreateAgentDialog( QDialog* parent = 0 );
    
    public slots:
        //! Fetch input data of agent and save to database.
        void SaveAgent();  
};

#endif
