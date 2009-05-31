/*!
 * \file  SettingsDialog.h
 * \brief Declarations for SettingsDialog.cpp
 *
 * \ingroup Main
 */

#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include "ui_SettingsDialog.h"

//! Declares members and slots for SettingsDialog.cpp
class SettingsDialog : public QDialog, public Ui::SettingsDialog {
        Q_OBJECT  // so that `moc` gets invoked

    public:
        //! Constructor
        SettingsDialog( QDialog* parent = 0 );

    public slots:
        //! Fetch input data of agent and save to database.
        void SaveSettings();
        void DataStore();
        void PreferredBrowser();
};

#endif
