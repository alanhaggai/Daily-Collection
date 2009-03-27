#ifndef CREATEDEBTORDIALOG_H
#define CREATEDEBTORDIALOG_H

#include "ui_createdebtordialog.h"
#include "dbconnect.h"

class CreateDebtorDialog : public QDialog, public Ui::CreateDebtorDialog {
    Q_OBJECT

    public:
        CreateDebtorDialog( QDialog *parent = 0 );

    public slots:
        void save(void);

    private:
        QSqlDatabase db;
        QMap<int, QString> agentMap;
};

#endif
