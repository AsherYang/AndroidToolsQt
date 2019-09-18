#ifndef COPYFILE_H
#define COPYFILE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QSizePolicy>
#include <QDebug>
#include "copyfilethread.h"

class CopyFile : public QWidget
{
    Q_OBJECT
public:
    explicit CopyFile(QWidget *parent = nullptr);
    void initUi();
    ~CopyFile();

signals:

public slots:
    void copyFileBtnClick();
    void deleteDirBtnClick();
    void openFileBtnClick();
    void setTips(QString text);
    void copyFileSuccess();

private:
    QVBoxLayout *mainLayout = nullptr;
    QHBoxLayout *operBtnsLayout = nullptr;
    QFormLayout *filePathForm = nullptr;
    QLabel *srcFilePathLabel = nullptr;
    QLabel *destFilePathLabel = nullptr;
    QLabel *fileListLabel = nullptr;
    QLineEdit *srcFilePathEdit = nullptr;
    QLineEdit *destFilePathEdit = nullptr;
    QTextEdit *fileListEdit = nullptr;
    QLabel *tipsLabel = nullptr;
    QPushButton *copyFileBtn = nullptr;
    QPushButton *deleteDirBtn = nullptr;
    QPushButton *openFileBtn = nullptr;
    CopyFileThread *cpFileThread = nullptr;
};

#endif // COPYFILE_H
