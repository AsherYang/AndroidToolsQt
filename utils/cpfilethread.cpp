#include "cpfilethread.h"
#include <QRegExp>
#include <QDebug>
#include "fileutil.h"

/*
CpFilethread::CpFilethread(const QString &srcDir, const QString &destDir, const QStringList &fileNameList) :
    isCopyFile(true),
    isCopyDir(false),
    copyResult(false)
{
    qSrcDir = srcDir;
    qDestDir = destDir;
    qFilePathList = fileNameList;
}
*/
CpFilethread::CpFilethread(const QString &srcDir, const QString &destDir, const bool &coverFileIfExist) :
    isCopyFile(false),
    isCopyDir(true),
    copyResult(false)
{
    qSrcDir = srcDir;
    qDestDir = destDir;
    needCoverFile = coverFileIfExist;
}

void CpFilethread::run()
{
    FileUtil fileUtil;
    if (isCopyFile && !qSrcDir.isEmpty() && !qDestDir.isEmpty() && !qFilePathList.isEmpty()) {
        foreach(QString filePath, qFilePathList) {
            // qDebug() << "filePath=" << filePath << ",!interruption=" << !isInterruptionRequested() << endl;;
            if (!isInterruptionRequested()) {
                copyResult = fileUtil.copyFile(qSrcDir.toStdString(), qDestDir.toStdString(), qSrcDir.toStdString(), filePath.toStdString());
            }
        }
    }

    if (isCopyDir && !qSrcDir.isEmpty() && !qDestDir.isEmpty()) {
        // qDebug() << "filePath=" << filePath << ",!interruption=" << !isInterruptionRequested() << endl;;
        if (!isInterruptionRequested()) {
            copyResult = fileUtil.copyDir(qSrcDir.toStdString(), qDestDir.toStdString(), needCoverFile);
        }
    }

    if (copyResult) {
        copySuccess();
    }
}

CpFilethread::~CpFilethread()
{
    requestInterruption();
    quit();
    wait();
}
