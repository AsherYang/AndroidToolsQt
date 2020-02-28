#ifndef RUNSYSCMD_H
#define RUNSYSCMD_H

#include <QString>
#include <QProcess>

class RunSysCmd
{
public:
    RunSysCmd();
    ~RunSysCmd();
    void callBatScript(const QString &batPath, const bool &showConsole=1);
};

#endif // RUNSYSCMD_H
