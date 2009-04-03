#ifndef AGENTEDITDIALOG_H
#define AGENTEDITDIALOG_H

#include "ui_EditAgentDialog.h"

class EditAgentDialog : public QDialog, public Ui::EditAgentDialog {
    Q_OBJECT

    public:
        EditAgentDialog( QDialog *parent = 0 );

    public slots:
        void fetchItem(QTableWidgetItem *);
        void populateTableWidget();
        void save();

    private:
        int currentRow;  // Holds the row of the selected item
};

#endif
