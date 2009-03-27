#ifndef DAYBOOKAGENTDIALOG_H
#define DAYBOOKAGENTDIALOG_H

#include "ui_daybookagentdialog.h"

class DaybookAgentDialog : public QDialog, public Ui::DaybookAgentDialog {
    Q_OBJECT

    public:
        DaybookAgentDialog( QDialog *parent = 0 );

    public slots:
        void populateTableWidget(int);

    private:
        QMap<int, QString> agentMap;
};

#endif
