#include "runsyscmd.h"
#include <QDebug>
#include "fileutil.h"
#include <Windows.h>

RunSysCmd::RunSysCmd()
{

}

/**
 * @brief RunSysCmd::callBatScript
 * @param batPath
 * https://blog.csdn.net/wayxl184/article/details/82875835
 *
 * 关于调用bat窗口显示问题，最终的处理是，用vbs将bat脚本封装，再由外部bat或vbs调用。
 * 比如 do_cplc.vbs 编写如下：
 * set ws=WScript.CreateObject("WScript.Shell")
 * ws.Run "do_cplc.bat",0
 */
void RunSysCmd::callBatScript(const QString &batPath, const bool &showConsole) {
    if (batPath.isEmpty()) {
        return;
    }
    FileUtil fileUtil;
    QString workPath = fileUtil.getFileDir(batPath.toStdString());
    if (showConsole) {
        QProcess p;
        p.setWorkingDirectory(workPath);
        p.start(batPath);
        p.waitForFinished();
        /*
        QByteArray qbt = p.readAllStandardOutput();
        QString msg = QString::fromLocal8Bit(qbt);
        qDebug() << "MSG:" << msg;
        */
    } else {
        /*
         * 1.这部分代码还没有调通,主要是path转换错误.
         * 2.窗口隐藏的问题：当指调用单个bat时，SW_HIDE方式能正常隐藏窗口；
         * 3.但当bat多层级联调用时，即bat里调用bat, 那通过外部SW_HIDE隐藏调用,内部级联的bat依然会出现cmd窗口；
         * 4.既然如此，那就直接将bat通过vbs封装起来，外部bat直接调用vbs，再由vbs调用bat脚本，这样才不会出现cmd窗口。
         * 5.通过vbs封装的方式，上面if中QProcess也能达到隐藏窗口的目的，因为源头是要将bat由vbs封装起来。
         */
        char *path = nullptr;
        QByteArray ba = batPath.toUtf8();
        //path = (char*) malloc(ba.length());
        //memset(path, 0, ba.length());
        //memcpy(path, ba.data(), ba.length());
        path = ba.data();
        //WinExec(path, SW_SHOWNORMAL);
        //WinExec("D:/xtc_resource/NFC/NFC资料汇总/ISD密钥工具/NXP_JCShell_4.7.0.1/batForTools/1_get_cplc.bat", SW_HIDE);
        WinExec(path, SW_HIDE);
        qDebug() << "path:" << QString::fromUtf8(path);
    }
}

RunSysCmd::~RunSysCmd() {

}
