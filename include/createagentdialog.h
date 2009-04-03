#ifndef CREATEAGENTDIALOG_H
#define CREATEAGENTDIALOG_H

#include "ui_createagentdialog.h"

class CreateAgentDialog : public QDialog, public Ui::CreateAgentDialog {
    Q_OBJECT

    public:
        CreateAgentDialog( QDialog *parent = 0 );
    
    public slots:
        void save();
};

#endif
