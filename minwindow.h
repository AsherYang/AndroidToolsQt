#ifndef MINWINDOW_H
#define MINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QMenuBar>

#include "nfc/nfc.h"

namespace Ui {
    class MainWindow;
}

class MinWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MinWindow(QWidget *parent = nullptr);
    ~MinWindow();

private:
    void resizeUi();
    void initUi();

private:
    Ui::MainWindow *ui;
    QWidget *centralWidget = nullptr;
    QVBoxLayout *mainLayout = nullptr;
    Nfc *nfc = nullptr;
};

#endif // MINWINDOW_H
