#include "mainwindow.h"
#include <QApplication>
#include "minwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //MinWindow w;
    //w.show();

    return a.exec();
}
