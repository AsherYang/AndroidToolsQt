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

    copyFileAction = new QAction("copyFile");
    copyFileAction->connect(copyFileAction, SIGNAL(triggered()), this, SLOT(openCopyFileView()));

    menuView->addAction(logKeywordAction);
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

    delete menuBar;
    delete stackedWidget;
    delete mainLayout;
    delete centralWidget;
    delete ui;
}
