#include "fileutil.h"

#include <QDebug>

FileUtil::FileUtil()
{

}

QString FileUtil::getFileName(string filePath)
{
    if (filePath.empty()) {
        return "";
    }
    /*
    StrUtil strutil;
    strutil.replace_all(filePath, "/", "\\");
    string::size_type pos = filePath.find_last_of("\\") + 1;
    return filePath.substr(pos, filePath.length() - pos);
    */

    QFileInfo fileInfo;
    fileInfo.setFile(QString::fromStdString(filePath));
    //qDebug() << "fileName=" << fileInfo.fileName() << ",baseName=" << fileInfo.baseName() << ",completeBaseName=" << fileInfo.completeBaseName() << ",suffix=" << fileInfo.suffix() << ",bundleName=" << fileInfo.bundleName() << ",completeSuffix=" << fileInfo.completeSuffix() << endl;
    //qDebug() << "path=" << fileInfo.path() << ",absolutePath=" + fileInfo.absolutePath() << ",absoluteFilePath=" << fileInfo.absoluteFilePath() << ",canonicalPath=" << fileInfo.canonicalPath() << ",canonicalFilePath=" << fileInfo.canonicalFilePath() << endl;
    return fileInfo.fileName();
}

QString FileUtil::getFileExt(string filePath)
{
    if (filePath.empty()) {
        return "";
    }
    /*
    string::size_type pos = filePath.find_last_of(".") + 1;
    return filePath.substr(pos, filePath.length() - pos);
    */

    QFileInfo fileInfo;
    fileInfo.setFile(QString::fromStdString(filePath));
    return fileInfo.suffix();
}

QString FileUtil::getFilePathWithName(string filePath)
{
    if (filePath.empty()) {
        return "";
    }
    /*
    string::size_type pos = filePath.find_last_of(".");
    return filePath.substr(0, pos);
    */

    QFileInfo fileInfo;
    fileInfo.setFile(QString::fromStdString(filePath));
    return QDir::toNativeSeparators(fileInfo.path().append(QDir::separator()).append(fileInfo.baseName()));
}

QString FileUtil::getFileDir(string filePath)
{
    if (filePath.empty()) {
        return "";
    }
    QFileInfo fileInfo;
    fileInfo.setFile(QString::fromStdString(filePath));
    return fileInfo.dir().absolutePath();
}

/**
 * may return empty vector
 * @brief FileUtil::getAllFiles
 * @param dir
 * @return all files under the dir and subDir
 */
QStringList FileUtil::getAllFiles(const string &dir)
{
    QStringList allFiles;
    if (dir.empty()) {
        return allFiles;
    }
    QFileInfo fileInfo;
    fileInfo.setFile(QString::fromStdString(dir));
    if (!fileInfo.isDir()) {
        return allFiles;
    }
    QDir fileDir(QString::fromStdString(dir));
    QString fileAbsPath;
    QString subDirPath;
    // 搜索当前符合条件的文件
    foreach(QString file, fileDir.entryList(QDir::Files)) {
        fileAbsPath = QDir::toNativeSeparators(QString::fromStdString(dir).append(QDir::separator()).append(file));
        allFiles += fileAbsPath;
    }

    // 搜索当前目录的子目录下符合条件的文件
    foreach(QString subdir, fileDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        subDirPath = QDir::toNativeSeparators(QString::fromStdString(dir).append(QDir::separator()).append(subdir));
        allFiles += getAllFiles(subDirPath.toStdString());
    }

    return allFiles;
}

QStringList FileUtil::getAllFiles(const string &dir, const QStringList &nameFilters)
{
    QStringList allFiles;
    if (dir.empty()) {
        return allFiles;
    }
    QFileInfo fileInfo;
    fileInfo.setFile(QString::fromStdString(dir));
    if (!fileInfo.isDir()) {
        return allFiles;
    }
    QDir fileDir(QString::fromStdString(dir));
    QString fileAbsPath;
    QString subDirPath;
    // 搜索当前符合条件的文件
    foreach(QString file, fileDir.entryList(nameFilters, QDir::Files)) {
        fileAbsPath = QDir::toNativeSeparators(QString::fromStdString(dir).append(QDir::separator()).append(file));
        allFiles += fileAbsPath;
    }

    // 搜索当前目录的子目录下符合条件的文件
    foreach(QString subdir, fileDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        subDirPath = QDir::toNativeSeparators(QString::fromStdString(dir).append(QDir::separator()).append(subdir));
        allFiles += getAllFiles(subDirPath.toStdString(), nameFilters);
    }

    return allFiles;
}

QStringList FileUtil::getSubDir(const string &dir)
{
    QStringList subDirList;
    if (dir.empty()) {
        return subDirList;
    }
    QFileInfo fileInfo;
    fileInfo.setFile(QString::fromStdString(dir));
    if (!fileInfo.isDir()) {
        return subDirList;
    }
    QDir fileDir(QString::fromStdString(dir));
    foreach (QString subDir, fileDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        subDirList += subDir;
    }
    return subDirList;
}

// mkdir|mkpath: {@see https://www.cnblogs.com/li-peng/p/3656866.html}
bool FileUtil::mkdirNotExist(const string &dir)
{
    if (dir.empty()) {
        return false;
    }
    QDir qDir;
    return qDir.mkpath(QString::fromStdString(dir));
}

// rmDir|rmpath: {@see https://doc-snapshots.qt.io/qt5-5.9/qdir.html#rmpath}
bool FileUtil::removeDirs(const string &dir)
{
    if (dir.empty()) {
        return false;
    }
    QDir qDir(QString::fromStdString(dir));
    if (!qDir.exists()) {
        return true;
    }
    return qDir.removeRecursively();
//    qDir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
//    QFileInfoList fileList = qDir.entryInfoList();
//    foreach (QFileInfo file, fileList) {
//        // 文件, 删除
//        if (file.isFile()) {
//            file.dir().remove(file.fileName());
//        } else {
//            // 递归删除
//            removeDirs(file.absolutePath().toStdString());
//        }
//    }
//    // 删除文件夹
//    return qDir.rmpath(qDir.absolutePath());
}

bool FileUtil::isFileOrDirExist(const string &filePath)
{
    QFileInfo fileInfo(QString::fromStdString(filePath));
    return fileInfo.exists();
}

// {@see https://blog.csdn.net/sinat_15433607/article/details/79863487}
bool FileUtil::copyFile(const string &srcDir, const string &destDir, const string &origSrcDir, const string &copyFileName)
{
    qDebug() << "---------------copyFile--------";
    bool result = false;
    QString srcFilePath;
    if (copyFileName.empty()) {
        srcFilePath = QString::fromStdString(srcDir);
    } else {
        QDir qSrcDir(QString::fromStdString(srcDir));
        srcFilePath = qSrcDir.absolutePath() + QDir::separator() + QString::fromStdString(copyFileName);
    }

    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir qDir(srcFilePath);
        QFileInfoList fileList = qDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
        foreach (QFileInfo file, fileList) {
            //qDebug() << "copy is dir ---" << "srcDir=" << srcFilePath << ",FileAbsPath" << QString::fromStdString(file.absolutePath().toStdString()) << ",fileName=" << file.fileName();
            result = copyFile(file.absolutePath().toStdString(), destDir, origSrcDir, file.fileName().toStdString());
        }
    } else {
        srcFilePath = QDir::toNativeSeparators(srcFilePath);
        QString qOrigSrcDir = QDir::toNativeSeparators(QString::fromStdString(origSrcDir));
        QString qDestDir = QDir::toNativeSeparators(QString::fromStdString(destDir));
        QString destFilePath = QString(srcFilePath).replace(qOrigSrcDir, qDestDir);
        //qDebug() << "copy srcFilePath=" << srcFilePath << " ,origSrcDir=" << qOrigSrcDir << " ,qDestDir=" << qDestDir;
        QFileInfo destFileInfo(destFilePath);
        QDir destDir = destFileInfo.dir();
        //qDebug() << "copy destDir=" << destDir << " ,destFilePath=" << destFilePath;
        if (!destDir.exists()) {
            destDir.mkpath(destDir.absolutePath());
        }
        //QFile srcFile(srcFilePath);
        srcFilePath.replace("\\", "/");
        destFilePath.replace("\\", "/");
        qDebug() << "copy srcFilePath" << srcFilePath << ",destFilePath" << destFilePath;
        result = QFile::copy(srcFilePath, destFilePath);
    }
    return result;
}

// {@see https://blog.csdn.net/ymc0329/article/details/7975654}
bool FileUtil::copyDir(const string &srcDir, const string &destDir, bool coverFileIfExist) {
    QString qSrcDir = QDir::toNativeSeparators(QString::fromStdString(srcDir));
    QString qDestDir = QDir::toNativeSeparators(QString::fromStdString(destDir));
    QDir sourceDir(qSrcDir);
    QDir targetDir(qDestDir);
    if (!targetDir.exists()) {
        if (!targetDir.mkdir(targetDir.absolutePath())) {
            return false;
        }
    }

    QFileInfoList fileList = sourceDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    foreach (QFileInfo file, fileList) {
        if (file.isDir()) {
            if (!copyDir(file.absolutePath().toStdString(), targetDir.filePath(file.fileName()).toStdString(), coverFileIfExist)) {
                return false;
            }
        } else {
            // remove first if coverFileIfExist
            if (coverFileIfExist && targetDir.exists(file.fileName())) {
                targetDir.remove(file.fileName());
            }
            // copy
            // qDebug() << "copy srcFilePath" << file.filePath() << ",destFilePath" << targetDir.filePath(file.fileName());
            if (!QFile::copy(file.filePath(), targetDir.filePath(file.fileName()))) {
                return false;
            }
        }
    }
    return true;
}

