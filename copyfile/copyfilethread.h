#ifndef COPYFILETHREAD_H
#define COPYFILETHREAD_H
#include <QThread>

class CopyFileThread : public QThread
{
    Q_OBJECT

public:
    CopyFileThread(const QString &srcDir, const QString &destDir, const QString &fileNameListStr);
    ~CopyFileThread();

signals:
    void copySuccess();

protected:
    virtual void run();

private:
    QString qSrcDir;
    QString qDestDir;
    QString qFileNameListStr;
    QStringList qFilePathList;
    bool copyResult;
};

#endif // COPYFILETHREAD_H
