#include "copyfilethread.h"
#include <QRegExp>
#include <QDebug>
#include "utils/fileutil.h"

CopyFileThread::CopyFileThread(const QString &srcDir, const QString &destDir, const QString &fileNameListStr)
{
    qSrcDir = srcDir;
    qDestDir = destDir;
    qFileNameListStr = fileNameListStr;
    copyResult = false;
}

void CopyFileThread::run()
{
    // 不同系统的换行符不一样，处理QString 需要兼容换行符 {@see https://www.cnblogs.com/foohack/p/4605588.html}
    // 所以单单使用 QRegExp re(".*/S"); 不能达到效果
    qFileNameListStr = qFileNameListStr.trimmed();
    qFilePathList = qFileNameListStr.split(QRegExp("[\r\n]"), QString::SkipEmptyParts);
//    qDebug() << "qStr=" << qFileNameListStr;
//    qDebug() << "qList=" << qFilePathList << endl;
    FileUtil fileUtil;
    foreach(QString filePath, qFilePathList) {
//        qDebug() << "filePath=" << filePath << ",!interruption=" << !isInterruptionRequested() << endl;;
        if (!isInterruptionRequested()) {
            copyResult = fileUtil.copyFile(qSrcDir.toStdString(), qDestDir.toStdString(), qSrcDir.toStdString(), filePath.toStdString());
        }
    }
    if (copyResult) {
        copySuccess();
    }
}

CopyFileThread::~CopyFileThread()
{
    requestInterruption();
    quit();
    wait();
}
