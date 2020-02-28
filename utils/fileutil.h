#ifndef FILEUTIL_H
#define FILEUTIL_H
#include <string>
#include <vector>
#include <QDir>
#include <QFileInfo>

#include "strutil.h"


class FileUtil
{
public:
    FileUtil();
    QString getFileName(string filePath);            // 从文件路径中提取文件名(包括后缀拓展名)
    QString getFileExt(string filePath);             // 返回文件拓展名(不包含点".")
    QString getFilePathWithName(string filePath);    // 返回文件名(包括文件目录, 但不包含扩展名)
    QString getFileDir(string filePath);             // 返回文件父目录
    QStringList getAllFiles(const string &dir);      // 获取指定目录及其子目录下所有文件集合
    QStringList getAllFiles(const string &dir, const QStringList &nameFilters);     // 获取指定目录及其子目录下, (包含过滤特殊关键字比如后缀名*.cpp)的所有文件集合
    QStringList getSubDir(const string &dir);               // 获取指定目录下的子一级目录
    bool mkdirNotExist(const string &dir);                  // 当文件目录不存在时，创建一个文件目录(创建多层目录)
    bool removeDirs(const string &dir);                     // 删除文件夹
    bool isFileOrDirExist(const string &filePath);          // 判断文件或文件夹是否存在
    bool copyFile(const string &srcDir, const string &destDir, const string &origSrcDir, const string &copyFileName);    // 将文件从源目录拷贝到目标目录下
    bool copyDir(const string &srcDir, const string &destDir, bool coverFileIfExist);       // 拷贝文件夹
};

/////////////////////////////////////////////////////////////////////
//  test code
//
//    FileUtil fileUtil;
//    string filePath = "G:\\copyfile\\234.txt";
//    string fileDir = "D:\\111111\\Test";
//    qDebug() << "fileName: " << fileUtil.getFileName(filePath) << endl;
//    qDebug() << "fileExt: " << fileUtil.getFileExt(filePath) << endl;
//    qDebug() << "filePathWithName: " << fileUtil.getFilePathWithName(filePath) << endl;
//    qDebug() << "fileDir: " << fileUtil.getFileDir(filePath) << endl;
//    QStringList files = fileUtil.getAllFiles(fileDir);
//    for (QStringList::iterator it = files.begin(); it != files.end(); ++it) {
//        qDebug() << "it=> " << *it << endl;
//    }
//    QStringList fileDirs = fileUtil.getSubDir(fileDir);
//    foreach (QString dir, fileDirs) {
//        qDebug() << "dir=" << dir;
//    }
//    for (QStringList::iterator it = fileDirs.begin(); it != fileDirs.end(); ++it) {
//        qDebug() << "---> dir2=" << *it;
//    }
//    QStringList allSubDirs = fileUtil.getAllSubDir(fileDir);
//    for (QStringList::iterator it = allSubDirs.begin(); it != allSubDirs.end(); ++it) {
//        qDebug() << "allSubDirs=" << *it << endl;
//    }
//    qDebug() << "mkPath:" << fileUtil.mkdirNotExist("D:\\111111\\22\\33\\44") << endl;
//    qDebug() << "exist:" << fileUtil.isFileOrDirExist("D:\\111111\\Test\\build.gradle") << endl;
//    qDebug() << "rmPath:" << fileUtil.removeDirs("D:\\111111\\22\\33\\44\\");
//    qDebug() << "copy:" << fileUtil.copyFile("D:\\111111\\22", "D:\\2", "D:\\111111\\22", "33");
//
///////////////////////////////////////////////////////////////////////////

#endif // FILEUTIL_H
