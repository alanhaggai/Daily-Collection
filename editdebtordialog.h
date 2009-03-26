#ifndef EDITDEBTORDIALOG_H
#define EDITDEBTORDIALOG_H

#include <QMap>

#include "ui_editdebtordialog.h"

class EditDebtorDialog : public QDialog, public Ui::EditDebtorDialog {
    Q_OBJECT

    public:
        EditDebtorDialog( QDialog *parent = 0 );

    public slots:
        void fetchItem(QTableWidgetItem *);
        void populateTableWidgetSerialEdit(const QString&);
        void saveDebtor();
        void deleteDebtor();
        void clearDebtor();

    private:
        int currentRow;  // Holds the row of the selected item
        QMap<int, QString> agentMap;
};

#endif
