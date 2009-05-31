/*!
 * \file  SettingsDialog.cpp
 * \brief Provides interface for changing settings.
 *
 * \ingroup Main
 */

#include "SettingsDialog.h"

#include <QMessageBox>
#include <QSettings>
#include <QDir>
#include <QFileDialog>

SettingsDialog::SettingsDialog(QDialog* parent)
        : QDialog(parent) {
    setupUi(this);

    QSettings settings;

    if ( settings.value("AutomaticBackup/enabled").toInt() )
        backup_enabled_check->setCheckState(Qt::Checked);

    if ( settings.value("AutomaticBackup/data_store").toString() != "" ) {
        data_store_edit->setText(
                settings.value("AutomaticBackup/data_store").toString() );
    }

    if ( settings.value("Browser/preferred").toString() != "" ) {
        browser_preferred_edit->setText(
                settings.value("Browser/preferred").toString() );
    }

    connect( save_button,     SIGNAL( clicked() ), this,
            SLOT( SaveSettings() ) );
    connect( data_store_tool, SIGNAL( clicked() ), this,
            SLOT( DataStore() ) );
    connect( browser_preferred_tool, SIGNAL( clicked() ), this,
            SLOT( PreferredBrowser() ) );
}

void
SettingsDialog::SaveSettings() {
    QSettings settings;
    settings.setValue( "AutomaticBackup/enabled",
            backup_enabled_check->checkState() );
    settings.setValue( "AutomaticBackup/data_store", data_store_edit->text() );
    settings.setValue( "Browser/preferred", browser_preferred_edit->text() );

    QMessageBox* msgbox = new QMessageBox( QMessageBox::Information,
            "Settings saved successfully",
            "Settings have been saved successfully.",
            QMessageBox::Ok );
    msgbox->exec();
}

void
SettingsDialog::DataStore() {
    QString directory = QFileDialog::getExistingDirectory( this, "Data store",
            QCoreApplication::applicationDirPath(), QFileDialog::ShowDirsOnly );

    if ( directory.isEmpty() ) 
        return;

    data_store_edit->setText(directory);
}

void
SettingsDialog::PreferredBrowser() {
    QString filename = QFileDialog::getOpenFileName( this,
            "Preferred web browser",
            QCoreApplication::applicationDirPath(),
#ifdef Q_OS_WIN32
            "Executable Files (*.exe)" );
#else
            "Executable Files (*)" );
#endif

    if ( filename.isEmpty() )
        return;

    browser_preferred_edit->setText(filename);
}
