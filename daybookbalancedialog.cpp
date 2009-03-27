#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "daybookbalancedialog.h"

DaybookBalanceDialog::DaybookBalanceDialog(QDialog *parent) : QDialog( parent, Qt::Tool ) {
    setupUi(this);

    QSqlQuery query;
    query.prepare("SELECT * FROM agent");
    if ( !query.exec() ) {
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }
    while ( query.next() ) {
         agentCombo->addItem( query.value(1).toString() );
         agentMap[query.value(0).toInt()] = query.value(1).toString();
    }
    agentCombo->setCurrentIndex(-1);
}
