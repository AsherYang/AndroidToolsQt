#ifndef CPFILETHREAD_H
#define CPFILETHREAD_H
#include <QThread>

class CpFilethread : public QThread
{
    Q_OBJECT

public:
    CpFilethread(const QString &srcDir, const QString &destDir, const QStringList &fileNameList);
    CpFilethread(const QString &srcDir, const QString &destDir, const bool &coverFileIfExist);
    ~CpFilethread();

signals:
    void copySuccess();

protected:
    virtual void run();

private:
    QString qSrcDir;
    QString qDestDir;
    QStringList qFilePathList;
    bool isCopyFile;
    bool isCopyDir;
    bool needCoverFile;
    bool copyResult;
};

#endif // COPYFILETHREAD_H
