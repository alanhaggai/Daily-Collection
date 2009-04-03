#include <QApplication>

#include "DbConnect.h"
#include "DailyCollectionWindow.h"

int main( int argc, char *argv[] ) {
    QApplication app( argc, argv );

    if (dbConnect()) {
        DailyCollectionWindow *window = new DailyCollectionWindow;
        window->show();
    }
    else {
        exit(0);
    }

    return app.exec();
}
