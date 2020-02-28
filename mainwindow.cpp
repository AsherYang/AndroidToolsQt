#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    centralWidget(new QWidget()),
    mainLayout(new QVBoxLayout()),
    stackedWidget(new QStackedWidget()),
    menuBar(new QMenuBar())
{
    ui->setupUi(this);
    resizeUi();
    initUi();
}

void MainWindow::resizeUi() {
     QDesktopWidget widget;
     QRect screen = widget.screenGeometry(this);
     resize(screen.width() / 8 * 3, screen.height() / 6 * 3);
}

void MainWindow::initUi() {
    setWindowTitle("AndroidTools");
    // local pointer variable
    QMenu *menuView;
    QMenu *menuTools;
    QMenu *menuSettings;

    // container Widget

    // menuBar
    menuView = menuBar->addMenu("&View");
    menuTools = menuBar->addMenu("&Tools");
    menuSettings = menuBar->addMenu("&Setting");

    logKeywordAction = new QAction("Keyword");
    logKeywordAction->connect(logKeywordAction, SIGNAL(triggered()), this, SLOT(openLogKeywordView()));

    copyFileAction = new QAction("CopyFile");
    copyFileAction->connect(copyFileAction, SIGNAL(triggered()), this, SLOT(openCopyFileView()));

    nfcAction = new QAction("Nfc");
    nfcAction->connect(nfcAction, SIGNAL(triggered()), this, SLOT(openNfcView()));

    menuView->addAction(logKeywordAction);
    menuView->addAction(nfcAction);
    menuTools->addAction(copyFileAction);

    mainLayout->addWidget(stackedWidget);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setMenuBar(menuBar);
}

// loadLogKeyword window
void MainWindow::openLogKeywordView() {
    if (logKeyword) {
        delete logKeyword;
        logKeyword = nullptr;
        //qDebug() << "openLogKeywordView twice, delete first." << endl;
    }
    logKeyword = new LogKeyword();
    logKeyword->setEdit(logKeyword->loadKeyword());
    stackedWidget->addWidget(logKeyword);
    stackedWidget->setCurrentWidget(logKeyword);
}

// copyfile window
void MainWindow::openCopyFileView() {
    if (copyFile) {
        delete copyFile;
        copyFile = nullptr;
        //qDebug() << "openCopyFileView twice, delete first." << endl;
    }
    copyFile = new CopyFile();
    stackedWidget->addWidget(copyFile);
    stackedWidget->setCurrentWidget(copyFile);
}

// open nfc window
void MainWindow::openNfcView() {
    if (nfc) {
        delete nfc;
        nfc = nullptr;
    }
    nfc = new Nfc();
    stackedWidget->addWidget(nfc);
    stackedWidget->setCurrentWidget(nfc);
}

MainWindow::~MainWindow()
{
    if (logKeywordAction) {
        delete logKeywordAction;
        logKeywordAction = nullptr;
    }

    if (logKeyword) {
        delete logKeyword;
        logKeyword = nullptr;
    }

    if (copyFileAction) {
        delete copyFileAction;
        copyFileAction = nullptr;
    }

    if (copyFile) {
        delete copyFile;
        copyFile = nullptr;
    }

    if (nfcAction) {
        delete nfcAction;
        nfcAction = nullptr;
    }

    if (nfc) {
        delete nfc;
        nfc = nullptr;
    }

    delete menuBar;
    delete stackedWidget;
    delete mainLayout;
    delete centralWidget;
    delete ui;
}
