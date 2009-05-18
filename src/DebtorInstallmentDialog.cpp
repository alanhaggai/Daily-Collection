/*!
 * \file  DebtorInstallmentDialog.cpp
 * \brief Interface for adding installment paid by debtors
 *
 * \ingroup Daybook
 */

#include "DebtorInstallmentDialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

enum
{
    NAME,
    AMOUNT,
};

int installment;

//! Constructor
DebtorInstallmentDialog::DebtorInstallmentDialog(QDialog* parent) :
        QDialog( parent, Qt::Tool )
{

    setupUi(this);  // Setup UI
    installment = 0;

    connect( serial_edit, SIGNAL( textChanged(const QString&) ), this,
            SLOT( PopulateValuesOnSerialChange(const QString&) ) );
    connect( clear_button, SIGNAL( clicked() ), this, SLOT( Clear() ) );
    connect( installment_edit,
            SIGNAL( textChanged(const QString&) ), this,
            SLOT( CalculateAmount(const QString&) ) );
    connect( fifty_radio, SIGNAL( toggled(bool) ), this,
            SLOT( FiftyRadioToggled(bool) ) );
    connect( hundred_radio, SIGNAL( toggled(bool) ), this,
            SLOT( HundredRadioToggled(bool) ) );
}

/*
 * This function executes when serial number is entered/changed. It fetches the
 * debtor's name, which corresponds to the serial number, and selects the
 * proper installment.
 */
void
DebtorInstallmentDialog::PopulateValuesOnSerialChange(const QString&
        debtor_serial)
{
    // Get name and amount received by the debtor
    QSqlQuery query;
    query.prepare("SELECT name, amount FROM debtor WHERE serial = :debtor_serial");
    query.bindValue( ":debtor_serial", debtor_serial );
    if ( !query.exec() )
    {
        // Query execution failed
        qDebug() << query.lastError();
        qFatal("Failed to execute query.");
    }
    if ( query.next() )
    {
        QString debtor_name;    // Stores debtor's name
        QString debtor_amount;  // Amount 

        debtor_name   = query.value(NAME).toString();
        debtor_amount = query.value(AMOUNT).toString();

        name_edit->setText(debtor_name);
        installment_edit->setText("1");

        if ( debtor_amount.toInt() <= 4500 )
        {
            fifty_radio->toggle();
            amount_edit->setText("50");
        }
        else if ( debtor_amount.toInt() <= 9000 )
        {
            hundred_radio->toggle();
            amount_edit->setText("100");
        }
    }
}

void
DebtorInstallmentDialog::CalculateAmount(const QString& number_of_installments)
{
    amount_edit->setText( QString::number( number_of_installments.toInt() *
            installment ) );
}

void
DebtorInstallmentDialog::FiftyRadioToggled(bool checked)
{
    if (checked)
    {
        installment = 50;

        if ( amount_edit->text() != "" ) {
            amount_edit->setText(
                    QString::number( installment_edit->text().toInt()
                        * installment ) );
        }
    }
}

void
DebtorInstallmentDialog::HundredRadioToggled(bool checked)
{
    if (checked)
    {
        installment = 100;

        if ( amount_edit->text() != "" ) {
            amount_edit->setText(
                    QString::number( installment_edit->text().toInt()
                        * installment ) );
        }

    }
}

void
DebtorInstallmentDialog::Clear()
{
    serial_edit->setText("");
    name_edit->setText("");
    amount_edit->setText("");
    installment_edit->setText("1");

    fifty_radio->setDown(false);
    hundred_radio->setDown(false);

    serial_edit->setFocus();
}
