#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QMenuBar>

#include "logkeyword/logkeyword.h"
#include "copyfile/copyfile.h"
#include "nfc/nfc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openLogKeywordView();
    void openCopyFileView();
    void openNfcView();

private:
    void resizeUi();
    void initUi();

private:
    Ui::MainWindow *ui;
    QWidget *centralWidget = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    QStackedWidget *stackedWidget = nullptr;
    LogKeyword *logKeyword = nullptr;
    QMenuBar *menuBar = nullptr;
    QAction *logKeywordAction = nullptr;
    CopyFile *copyFile = nullptr;
    QAction *copyFileAction = nullptr;
    Nfc *nfc = nullptr;
    QAction *nfcAction = nullptr;
};

#endif // MAINWINDOW_H
