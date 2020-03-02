#include "runsyscmdthread.h"
#include "runsyscmd.h"
//#include <QDebug>

RunSysCmdThread::RunSysCmdThread(const QString &batPath, const bool &showConsole) :
    mBatPath(batPath),
    mShowConsole(showConsole)
{

}

void RunSysCmdThread::run() {
    //qDebug() << "current Thread:" << QThread::currentThread() << " ,Id:" << QThread::currentThreadId();
    RunSysCmd runCmd;
    runCmd.callBatScript(mBatPath, mShowConsole);
}

RunSysCmdThread::~RunSysCmdThread() {
    requestInterruption();
    quit();
    wait();
}
