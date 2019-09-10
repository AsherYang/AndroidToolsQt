#ifndef LOGKEYWORD_H
#define LOGKEYWORD_H

#include <QWidget>
#include <QMainWindow>
#include <QVBoxLayout>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qdebug.h>
#include <string>

using namespace std;

namespace Ui {
    class LogKeyword;
}

class LogKeyword : public QMainWindow {
    Q_OBJECT

public:
    explicit LogKeyword(QMainWindow *parent = nullptr);
    QString loadKeyword();
    void setEdit(QString str);
    ~LogKeyword();

private:
    void initUi();

private slots:
    void searchEditChange(QString str);

private:
    Ui::LogKeyword *ui;
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QLineEdit   *searchEdit;
    QTextEdit   *edit;
    vector<string> filterList;
    const string default_dir_path = "d:\\python_demo\\qt5\\logAnalytics\\dist\\";
    const string default_file_path = default_dir_path + "filter_config";
};


#endif  // LOGKEYWORD_H
