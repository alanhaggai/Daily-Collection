#ifndef DAYBOOKALLAGENTSDIALOG_H
#define DAYBOOKALLAGENTSDIALOG_H

#include "ui_DaybookAllAgentsDialog.h"

class DaybookAllAgentsDialog : public QDialog, public Ui::DaybookAllAgentsDialog {
    public:
        DaybookAllAgentsDialog( QDialog *parent = 0 );

        void populateTableWidget();
};

#endif
