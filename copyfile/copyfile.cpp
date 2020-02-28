#include "copyfile.h"
#include "utils/fileutil.h"
#include <QDesktopServices>
#include <QUrl>

#pragma execution_character_set("utf-8")

using namespace std;

CopyFile::CopyFile(QWidget *parent) :
    QWidget(parent),
    mainLayout(new QVBoxLayout()),
    operBtnsLayout(new QHBoxLayout()),
    filePathForm(new QFormLayout()),
    srcFilePathLabel(new QLabel()),
    destFilePathLabel(new QLabel()),
    fileListLabel(new QLabel()),
    srcFilePathEdit(new QLineEdit()),
    destFilePathEdit(new QLineEdit()),
    fileListEdit(new QTextEdit()),
    tipsLabel(new QLabel()),
    copyFileBtn(new QPushButton()),
    deleteDirBtn(new QPushButton()),
    openFileBtn(new QPushButton())
{
    initUi();
}

void CopyFile::initUi()
{
    srcFilePathLabel->setText(QString::fromUtf8("源根目录:"));
    destFilePathLabel->setText(QString::fromUtf8("目标目录:"));
    fileListLabel->setText(QString::fromUtf8("文件集合:"));
    copyFileBtn->setText(QString::fromUtf8("拷贝文件"));
    deleteDirBtn->setText(QString::fromUtf8("删除目录"));
    openFileBtn->setText(QString::fromUtf8("打开文件"));

    srcFilePathLabel->setMinimumHeight(25);
    destFilePathLabel->setMinimumHeight(25);
    srcFilePathEdit->setMinimumHeight(25);
    destFilePathEdit->setMinimumHeight(25);
    copyFileBtn->setFixedSize(100, 30);
    deleteDirBtn->setFixedSize(100, 30);
    openFileBtn->setFixedSize(100, 30);

    srcFilePathEdit->setPlaceholderText("Y:\\work_src\\gitlab\\I13\\HLOS");
    destFilePathEdit->setPlaceholderText("G:\\copyfile");

    copyFileBtn->connect(copyFileBtn, SIGNAL(clicked()), this, SLOT(copyFileBtnClick()));
    deleteDirBtn->connect(deleteDirBtn, SIGNAL(clicked()), this, SLOT(deleteDirBtnClick()));
    openFileBtn->connect(openFileBtn, SIGNAL(clicked()), this, SLOT(openFileBtnClick()));

    filePathForm->addRow(srcFilePathLabel, srcFilePathEdit);
    filePathForm->addRow(destFilePathLabel, destFilePathEdit);
    filePathForm->addRow(fileListLabel, fileListEdit);

    // 将表单最后一个文本框设置为可垂直拉伸 QSizePolicy
    QSizePolicy policy = fileListEdit->sizePolicy();
    policy.setVerticalStretch(1);
    fileListEdit->setSizePolicy(policy);

    operBtnsLayout->addWidget(tipsLabel);
    operBtnsLayout->addStretch(1);
    operBtnsLayout->addWidget(copyFileBtn);
    operBtnsLayout->addWidget(deleteDirBtn);
    operBtnsLayout->addWidget(openFileBtn);

    mainLayout->addLayout(filePathForm);
    mainLayout->addLayout(operBtnsLayout);

    setLayout(mainLayout);
}

void CopyFile::copyFileBtnClick()
{
    QString srcFilePath = srcFilePathEdit->text();
    QString destFilePath = destFilePathEdit->text();
    QString fileList = fileListEdit->toPlainText();
    //qDebug() << "srcFilePath: " << srcFilePath.isNull() << srcFilePath.isEmpty() << endl;
    if (fileList.isEmpty()) {
        QString tips = QString::fromUtf8("请在文件集合框中输入拷贝文件路径");
        setTips(tips);
        return;
    }
    if (srcFilePath.isEmpty()) {
        srcFilePath = QString::fromUtf8("Y:\\work_src\\gitlab\\I13\\HLOS");
    }
    if (destFilePath.isEmpty()) {
        destFilePath = QString::fromUtf8("G:\\copyfile");
    }
    srcFilePath = srcFilePath.trimmed();
    destFilePath = destFilePath.trimmed();
    if (cpFileThread) {
        QObject::disconnect(cpFileThread, SIGNAL(copySuccess(QString)), this, SLOT(setTips(QString)));
        delete cpFileThread;
        cpFileThread = nullptr;
    }
    cpFileThread = new CopyFileThread(srcFilePath, destFilePath, fileList);
    QObject::connect(cpFileThread, SIGNAL(copySuccess()), this, SLOT(copyFileSuccess()));
    cpFileThread->start();
}

void CopyFile::deleteDirBtnClick()
{
    QString destFilePath = destFilePathEdit->text();
    if (destFilePath.isEmpty()) {
        destFilePath = QString::fromUtf8("G:\\copyfile");
    }
    destFilePath = destFilePath.trimmed();
    FileUtil fileUtil;
    fileUtil.removeDirs(destFilePath.toStdString());
}

void CopyFile::openFileBtnClick()
{
    QString srcFilePath = srcFilePathEdit->text();
    QString openPath;
    if (!srcFilePath.isEmpty() && !srcFilePath.contains("work_src")) {
        if (srcFilePath.startsWith("\\") || srcFilePath.startsWith("/")) {
            openPath = QDir::toNativeSeparators("Y:\\work_src\\gitlab\\I13\\HLOS");
        } else {
            openPath = QDir::toNativeSeparators("Y:\\work_src\\gitlab\\I13\\HLOS\\");
        }
        openPath += QDir::toNativeSeparators(srcFilePath);
    } else if (!srcFilePath.isEmpty() && srcFilePath.contains("work_src")) {
        openPath = QDir::toNativeSeparators(srcFilePath);
    } else{
        openPath = "G:\\copyfile";
    }
    //qDebug() << "openFileBtnClick openPath=" << openPath;
    QFile openFile(openPath);
    if (!openFile.exists()) {
        return;
    }
    openPath = "file:///" + openPath;
    QDesktopServices::openUrl(QUrl::fromLocalFile(openPath));
}

void CopyFile::copyFileSuccess()
{
    setTips("copy file success");
    QString destFilePath = destFilePathEdit->text();
    QString openPath = "G:\\copyfile";
    if (!destFilePath.isEmpty()) {
        openPath = QDir::toNativeSeparators(destFilePath);
    }
    //qDebug() << "copyFileSuccess openPath=" << openPath;
    QFile openFile(openPath);
    if (!openFile.exists()) {
        return;
    }
    openPath = "file:///" + openPath;
    QDesktopServices::openUrl(QUrl::fromLocalFile(openPath));
}

void CopyFile::setTips(QString text)
{
    tipsLabel->setText(text);
}

CopyFile::~CopyFile()
{
    if (cpFileThread) {
        delete cpFileThread;
        cpFileThread= nullptr;
    }
    delete srcFilePathLabel;
    delete destFilePathLabel;
    delete fileListLabel;
    delete srcFilePathEdit;
    delete destFilePathEdit;
    delete fileListEdit;
    delete tipsLabel;
    delete copyFileBtn;
    delete deleteDirBtn;
    delete openFileBtn;
    delete operBtnsLayout;
    delete filePathForm;
    delete mainLayout;
}


