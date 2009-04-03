#ifndef DAILYCOLLECTIONWINDOW_H
#define DAILYCOLLECTIONWINDOW_H

#include "ui_dailycollectionwindow.h"

class DailyCollectionWindow : public QMainWindow, public Ui::DailyCollectionWindow {
    Q_OBJECT

    public:
        DailyCollectionWindow( QMainWindow *parent = 0 );

    public slots:
        void spawnCreateAgent();
        void spawnCreateDebtor();
        void spawnEditAgent();
        void spawnEditDebtor();
        void spawnEditTransaction();
        void spawnDaybookAgent();
        void spawnDaybookAllAgents();
        void spawnDaybookBalance();
        void spawnDebtorInstallment();
};

#endif
