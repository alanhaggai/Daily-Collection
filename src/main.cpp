/*!
 * \file    main.cpp
 * \author  <a href='http://alanhaggai.org/'>Alan Haggai Alavi</a>
 * \version 0.02
 *
 * \brief   Fires up the application.
 *
 * \defgroup DailyCollection DailyCollection Menu Items
 * \defgroup Main            Main
 * \defgroup Create          Create Menu Items
 * \defgroup Edit            Edit Menu Items
 * \defgroup Daybook         Daybook Menu Items
 * \defgroup Debtor          Debtor Menu Items
 * \defgroup About           About Menu Items
 *
 * \mainpage Daily Collection
 *
 * \a Daily \a Collection is a software which manages the daily accounting
 * activity of small banks
 * 
 * Initial version (0.01) of the software was written in
 * <a href='http://perl.org/'>Perl</a> by making use of
 * <a href='http://wxperl.sourceforge.net/'>WxPerl</a> which is a port of
 * <a href='http://wxwidgets.org/'>WxWidgets</a>. The software has been
 * rewritten in <a href='http://www.qtsoftware.com/products/'>Qt 4.5</a> as
 * there was much difficulty in packaging the earlier software.
 *
 * This version includes the fix for the infamous bug in Daybook/Balance.pm
 * which resulted in several queries being executed in a loop and thus slowing
 * the entire application, which could be mistook for a crash.
 *
 * <tt>-- The difference makes the difference --
 */
#include <QApplication>

#include "DbConnect.h"
#include "DailyCollectionWindow.h"

//! Giving life to the application.
int
main( int argc, char *argv[] ) {
    QApplication app( argc, argv );  // Create a QApplication object by passing
    // commandline arguments.

    app.setApplicationName("Daily Collection");
    app.setApplicationVersion("0.02");
    app.setOrganizationDomain("http://cubegin.com/");
    app.setOrganizationName("CubeGin");

    if ( DbConnect::Connect() ) {
            DailyCollectionWindow *window = new DailyCollectionWindow;
            window->show();  // Display the created window.
        }
    else
        exit(1);

    return app.exec();
}
