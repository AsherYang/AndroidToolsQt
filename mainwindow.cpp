#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    centralWidget(new QWidget(this)),
    mainLayout(new QVBoxLayout(this)),
    stackedWidget(new QStackedWidget(this)),
    menuBar(new QMenuBar(this))
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
    // container Widget
    logKeyword = new LogKeyword(this);

    menuView = menuBar->addMenu("&View");
    menuTools = menuBar->addMenu("&Tools");
    menuSettings = menuBar->addMenu("&Setting");

    logKeywordAction = new QAction("Keyword");
    menuView->addAction(logKeywordAction);
    connect(logKeywordAction, SIGNAL(triggered()), SLOT(openLogKeywordView()));

    mainLayout->addWidget(stackedWidget);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
    setMenuBar(menuBar);
}

// loadLogKeyword window
void MainWindow::openLogKeywordView() {
    logKeyword->setEdit(logKeyword->loadKeyword());
    stackedWidget->addWidget(logKeyword);
    stackedWidget->setCurrentWidget(logKeyword);
}

MainWindow::~MainWindow()
{
    if (logKeywordAction) {
        delete logKeywordAction;
        logKeywordAction= nullptr;
    }

    if (logKeyword) {
        delete logKeyword;
        logKeyword= nullptr;
    }

    if (menuView) {
        delete menuView;
        menuView= nullptr;
    }
    if (menuTools) {
        delete menuTools;
        menuTools= nullptr;
    }
    if (menuSettings) {
        delete menuSettings;
        menuSettings= nullptr;
    }

    delete ui;
    delete menuBar;
    delete mainLayout;
    delete stackedWidget;
    delete centralWidget;
}
