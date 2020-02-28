#include "minwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>


MinWindow::MinWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow()),
    centralWidget(new QWidget()),
    mainLayout(new QVBoxLayout())
{
    ui->setupUi(this);
    resizeUi();
    initUi();
}

void MinWindow::resizeUi() {
     QDesktopWidget widget;
     QRect screen = widget.screenGeometry(this);
     resize(screen.width() / 8 * 3, screen.height() / 6 * 3);
}

void MinWindow::initUi() {
    setWindowTitle("NfcTools");
    // container Widget
    nfc = new Nfc();
    mainLayout->addWidget(nfc);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

MinWindow::~MinWindow()
{
    if (nfc) {
        delete nfc;
        nfc = nullptr;
    }
    delete mainLayout;
    delete centralWidget;
    delete ui;
}
