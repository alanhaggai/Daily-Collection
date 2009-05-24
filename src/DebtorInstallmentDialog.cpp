/*!
 * \file  DebtorInstallmentDialog.cpp
 * \brief Interface for adding installment paid by debtors
 *
 * \ingroup Daybook
 */

#include "DebtorInstallmentDialog.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

enum {
    NAME,
    AMOUNT,
};

int installment;

//! Constructor
DebtorInstallmentDialog::DebtorInstallmentDialog(QDialog* parent) :
        QDialog(parent) {

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
    connect( save_button, SIGNAL( clicked() ), this,
            SLOT( SaveInstallment() ) );
}

/*
 * This function executes when serial number is entered/changed. It fetches the
 * debtor's name, which corresponds to the serial number, and selects the
 * proper installment.
 */
void
DebtorInstallmentDialog::PopulateValuesOnSerialChange(const QString&
        debtor_serial) {
    // Get name and amount received by the debtor
    QSqlQuery query;
    query.prepare("SELECT name, amount FROM debtor WHERE serial = :debtor_serial");
    query.bindValue( ":debtor_serial", debtor_serial );

    if ( !query.exec() ) {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Critical, "Query Execution Failed",
                "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                QMessageBox::Ok );
            msgbox->exec();

            return;
        }

    if ( query.next() ) {
            QString debtor_name;    // Stores debtor's name
            QString debtor_amount;  // Amount

            debtor_name   = query.value(NAME).toString();
            debtor_amount = query.value(AMOUNT).toString();

            name_edit->setText(debtor_name);
            installment_edit->setText("1");

            if ( debtor_amount.toInt() <= 4500 ) {
                    fifty_radio->toggle();
                    amount_edit->setText("50");
                }
            else if ( debtor_amount.toInt() <= 9000 ) {
                    hundred_radio->toggle();
                    amount_edit->setText("100");
                }
        }
}

/**
 * Calculate the amount which will be paid then
 */
void
DebtorInstallmentDialog::CalculateAmount(const QString& number_of_installments) {
    amount_edit->setText( QString::number( number_of_installments.toInt() *
            installment ) );
}

/**
 * Change installment to 50 and recalculate amount
 */
void
DebtorInstallmentDialog::FiftyRadioToggled(bool checked) {
    if (checked) {
            installment = 50;

            if ( amount_edit->text() != "" ) {
                    amount_edit->setText(
                        QString::number( installment_edit->text().toInt()
                                * installment ) );
                }
        }
}

/**
 * Change installment to 100 and recalculate amount
 */
void
DebtorInstallmentDialog::HundredRadioToggled(bool checked) {
    if (checked) {
            installment = 100;

            if ( amount_edit->text() != "" ) {
                    amount_edit->setText(
                        QString::number( installment_edit->text().toInt()
                                * installment ) );
                }

        }
}

/**
 * Clear fields
 */
void
DebtorInstallmentDialog::Clear() {
    serial_edit->setText("");
    name_edit->setText("");
    amount_edit->setText("");
    installment_edit->setText("1");

    fifty_radio->setDown(false);
    hundred_radio->setDown(false);

    serial_edit->setFocus();
}

/**
 * Save the installment made to table transaction
 */
void
DebtorInstallmentDialog::SaveInstallment() {
    if ( serial_edit->text() != "" && name_edit->text() != "" ) {
            qint16 amount, paid, amount_rented;

            ( fifty_radio->isChecked() == true )
            ? ( amount = 50 )
            : ( amount = 100 );

            // Calculate amout which will be paid
            if ( installment_edit->text() != "" ) {
                    amount *= installment_edit->text().toInt();
                }

            paid = amount;

            QSqlQuery query;
            query.prepare( "SELECT SUM(transaction.paid) FROM debtor, transaction\
                WHERE debtor.id = transaction.debtor_id AND debtor.serial = :debtor_serial" );
            query.bindValue( ":debtor_serial", serial_edit->text() );

            if ( !query.exec() ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Critical, "Query Execution Failed",
                        "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                        QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }

            query.next();

            QString sum = query.value(0).toString();

            query.prepare( "SELECT id, agent_id FROM debtor WHERE debtor.serial = :debtor_serial" );
            query.bindValue( ":debtor_serial", serial_edit->text() );

            if ( !query.exec() ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Critical, "Query Execution Failed",
                        "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                        QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }

            query.next();

            QString debtor_id = query.value(0).toString();
            QString agent_id  = query.value(1).toString();

            query.prepare( "INSERT INTO transaction ( agent_id, debtor_id, paid,\
            date ) VALUES ( :agent_id, :debtor_id, :paid, :date )" );

            query.bindValue( ":agent_id", agent_id );
            query.bindValue( ":debtor_id", debtor_id );
            query.bindValue( ":paid", amount );
            query.bindValue( ":date", date_calendar->selectedDate().toString(Qt::ISODate) );

            if ( !query.exec() ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Critical, "Query Execution Failed",
                        "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                        QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }

            query.prepare( "SELECT amount FROM debtor WHERE id = :id" );
            query.bindValue( ":id", debtor_id );

            if ( !query.exec() ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Critical, "Query Execution Failed",
                        "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                        QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }

            query.next();

            amount_rented = query.value(0).toInt();

            if ( amount_rented == amount ) {
                    query.prepare( "UPDATE debtor SET deleted = 1 WHERE id = :id" );
                    query.bindValue( ":id", debtor_id );
                }

            if ( !query.exec() ) {
                    QMessageBox* msgbox = new QMessageBox(
                        QMessageBox::Critical, "Query Execution Failed",
                        "Execution of query <b>" + query.lastQuery() + "</b>, failed.\n\nMost probably, MySQL server was not started.",
                        QMessageBox::Ok );
                    msgbox->exec();

                    return;
                }

            Clear();
        }
    else {
            QMessageBox* msgbox = new QMessageBox(
                QMessageBox::Warning, "Incomplete Fields",
                "All fields are to be filled.", QMessageBox::Ok );
            msgbox->exec();  // Fire up a message box
        }
}
