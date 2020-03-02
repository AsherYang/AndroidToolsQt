#ifndef RUNSYSCMDTHREAD_H
#define RUNSYSCMDTHREAD_H
#include <QThread>

class RunSysCmdThread : public QThread
{
    Q_OBJECT

public:
    RunSysCmdThread(const QString &batPath, const bool &showConsole=1);
    ~RunSysCmdThread();

protected:
    virtual void run();

private:
    QString mBatPath;
    bool mShowConsole;
};

#endif // RUNSYSCMDTHREAD_H
