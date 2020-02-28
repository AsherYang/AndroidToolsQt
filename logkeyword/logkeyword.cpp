#include "logkeyword.h"
#include <iostream>
#include <fstream>
#include "../utils/strutil.h"

#pragma execution_character_set("utf-8")

LogKeyword::LogKeyword(QMainWindow *parent) :
    QMainWindow (parent),
    centralWidget(new QWidget()),
    mainLayout(new QVBoxLayout()),
    searchEdit(new QLineEdit()),
    edit(new QTextEdit())
{
    initUi();
}

void LogKeyword::initUi()
{
    searchEdit->setMinimumHeight(23);
    setGeometry(300, 300, 370, 420);
    setWindowTitle("Filter");
    connect(searchEdit, SIGNAL(textChanged(const QString &)), this, SLOT(searchEditChange(const QString &)));

    mainLayout->addWidget(searchEdit);
    mainLayout->addWidget(edit);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void LogKeyword::setEdit(QString str)
{
    if (str.isEmpty()) {
        return;
    }
    edit->setText(str);
}

QString LogKeyword::loadKeyword()
{
    // read keyword from file.
    ifstream inFile;
    string originalData;
    inFile.open(default_file_path, ios::in);
    getline(inFile, originalData);
    // close file.
    inFile.close();
    StrUtil strUtil;
    filterList = strUtil.split(originalData, "#@$");
    // qDebug() << filterList.size() << endl;
    strUtil.replace_all(originalData, "#@$", "\n");
    return QString::fromStdString(originalData);
}

void LogKeyword::searchEditChange(QString str)
{
    if (filterList.size() == 0) {
        return;
    }

    QString editText;
    if(str.isEmpty()) {
        for (vector<string>::iterator it = filterList.begin(); it < filterList.end(); it++) {
            editText.append(QString::fromStdString(*it)).append("\n");
        }
    } else {
        string tmp = str.toStdString();
        // iterator the vector
        for (vector<string>::iterator it = filterList.begin(); it < filterList.end(); it++) {
            // find the str
            if (it->find(tmp) != string::npos) {
                //qDebug() << QString::fromStdString(*it) << endl;
                editText.append(QString::fromStdString(*it)).append("\n");
            }
        }
    }

    setEdit(editText);
}

LogKeyword::~LogKeyword()
{
    delete searchEdit;
    delete edit;
    delete mainLayout;
    delete centralWidget;
}
