#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

#include "daybookagentdialog.h"

DaybookAgentDialog::DaybookAgentDialog(QDialog *parent) : QDialog(parent) {
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
