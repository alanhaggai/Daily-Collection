#ifndef DAYBOOKAGENTDIALOG_H
#define DAYBOOKAGENTDIALOG_H

#include "ui_daybookagentdialog.h"

class DaybookAgentDialog : public QDialog, public Ui::DaybookAgentDialog {
    public:
        DaybookAgentDialog( QDialog *parent = 0 );

    private:
        QMap<int, QString> agentMap;
};

#endif
