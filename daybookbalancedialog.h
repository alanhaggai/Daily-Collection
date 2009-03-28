#ifndef DAYBOOKBALANCEDIALOG_H
#define DAYBOOKBALANCEDIALOG_H

#include "ui_daybookbalancedialog.h"

class DaybookBalanceDialog : public QDialog, public Ui::DaybookBalanceDialog {
    Q_OBJECT

    public:
        DaybookBalanceDialog( QDialog *parent = 0 );

    public slots:
        void populateTableWidget();

    private:
        QMap<int, QString> agentMap;
};

#endif
