#include "mainwindow.h"
#include <QApplication>
#include "minwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (a.applicationName().contains("AndroidTools")) {
        MainWindow w;
        w.show();
        return a.exec();
    } else {
        MinWindow w;
        w.show();
        return a.exec();
    }
}
