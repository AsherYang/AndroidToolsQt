#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QMenuBar>

#include "logkeyword/logkeyword.h"

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

private:
    void resizeUi();
    void initUi();

private:
    Ui::MainWindow *ui;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QStackedWidget *stackedWidget;
    LogKeyword *logKeyword;
    QMenuBar *menuBar;
    QAction *logKeywordAction;
};

#endif // MAINWINDOW_H
