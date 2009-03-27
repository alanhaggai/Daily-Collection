#ifndef DAYBOOKBALANCEDIALOG_H
#define DAYBOOKBALANCEDIALOG_H

#include "ui_daybookbalancedialog.h"

class DaybookBalanceDialog : public QDialog, public Ui::DaybookBalanceDialog {
    public:
        DaybookBalanceDialog( QDialog *parent = 0 );

    private:
        QMap<int, QString> agentMap;
};

#endif
