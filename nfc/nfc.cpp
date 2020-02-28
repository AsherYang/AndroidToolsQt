#include "nfc.h"
#include <QRegExp>
#include <QFile>
#include <QFileDialog>
#include <QResource>
#include "utils/fileutil.h"

#pragma execution_character_set("utf-8")

Nfc::Nfc(QWidget *parent) :
    QWidget(parent),
    mainLayout(new QVBoxLayout()),
    jcshellBatPathLayout(new QHBoxLayout()),
    serverAuthKeyPathLayout(new QHBoxLayout()),
    operBtnsLayout(new QHBoxLayout()),
    jcshellBatPathEdit(new QLineEdit()),
    jcshellBatPathBtn(new QPushButton()),
    serverAuthKeyPathEdit(new QLineEdit()),
    serverAuthKeyPathBtn(new QPushButton()),
    logEdit(new QTextEdit()),
    getCplcBtn(new QPushButton()),
    writeAuthKeyBtn(new QPushButton()),
    checkAuthKeyBtn(new QPushButton()),
    fileWatcher(new QFileSystemWatcher()),
    progressBar(new QProgressBar()) {

    initUi();
}

/**
 * @brief Nfc::initUi
 */
void Nfc::initUi() {
    getCplcBtn->setText(QString::fromUtf8("获取CPLC"));
    getCplcBtn->setFixedSize(100, 30);
    getCplcBtn->connect(getCplcBtn, SIGNAL(clicked()), this, SLOT(getCplcBtnClick()));

    writeAuthKeyBtn->setText(QString::fromUtf8("写入密钥"));
    writeAuthKeyBtn->setFixedSize(100, 30);
    writeAuthKeyBtn->connect(writeAuthKeyBtn, SIGNAL(clicked()), this, SLOT(writeAuthKeyBtnClick()));

    checkAuthKeyBtn->setText(QString::fromUtf8("校验密钥"));
    checkAuthKeyBtn->setFixedSize(100, 30);
    checkAuthKeyBtn->connect(checkAuthKeyBtn, SIGNAL(clicked()), this, SLOT(checkAuthKeyBtnClick()));

    serverAuthKeyPathBtn->setText(QString::fromUtf8("导入密钥文件"));
    serverAuthKeyPathBtn->setMinimumHeight(25);
    serverAuthKeyPathBtn->connect(serverAuthKeyPathBtn, SIGNAL(clicked()), this, SLOT(serverAuthKeyBtnClick()));
    serverAuthKeyPathEdit->setTextMargins(10, 0, 10, 0);

    jcshellBatPathBtn->setText(QString::fromUtf8("设置Jcshell脚本"));
    jcshellBatPathBtn->setMinimumHeight(25);
    jcshellBatPathBtn->connect(jcshellBatPathBtn, SIGNAL(clicked()), this, SLOT(jcshellBatPathBtnClick()));
    jcshellBatPathEdit->setTextMargins(10, 0, 10, 0);

    QSizePolicy policy = logEdit->sizePolicy();
    policy.setVerticalStretch(1);
    logEdit->setSizePolicy(policy);
    logEdit->setPlaceholderText(QString::fromUtf8("1.请先确认手表是否支持NFC，并且是Userdebug版本，\n2.从服务器获取测试密钥文件，\n3.确保手表连接稳定。"));
    // progress bar hidden.
    showBusy(false);

    serverAuthKeyPathLayout->addWidget(serverAuthKeyPathEdit);
    serverAuthKeyPathLayout->addWidget(serverAuthKeyPathBtn);

    jcshellBatPathLayout->addWidget(jcshellBatPathEdit);
    jcshellBatPathLayout->addWidget(jcshellBatPathBtn);

//    getCplcBtn->setLayoutDirection(Qt::LayoutDirection::RightToLeft);

    operBtnsLayout->addWidget(progressBar);
    operBtnsLayout->addStretch(1);
    operBtnsLayout->addWidget(getCplcBtn);
    operBtnsLayout->addWidget(writeAuthKeyBtn);
    operBtnsLayout->addWidget(checkAuthKeyBtn);
    mainLayout->addLayout(jcshellBatPathLayout);
    mainLayout->addLayout(serverAuthKeyPathLayout);
    mainLayout->addWidget(logEdit);
    mainLayout->addLayout(operBtnsLayout);
    setLayout(mainLayout);
}

/**
 * @brief Nfc::showLog
 * @param log
 */
void Nfc::showLog(QString log) {
    if (log.isEmpty()) {
        return;
    }
    logEdit->append(log);
}

/**
 * @brief Nfc::getCplcBtnClick
 * get cplc btn click slotOL
 */
void Nfc::getCplcBtnClick() {
    if (!checkJcshell()) {
        showLog(QString::fromUtf8("请先设置Jcshell脚本路径"));
        return;
    }
    //qDebug() << "---getCplcBtnClick---";
    //1_get_cplc.bat中的语句全为调用次级vbs脚本，再由vbs调用子bat脚本，从而达到隐藏窗口的目的
    showLog(QString::fromUtf8("正在获取CPLC，请稍后"));
    showBusy(true);
    FileUtil fileUtil;
    QString workPath;
    workPath.append(jcshellDirPath).append("/batForTools/");
    QString batPath;
    batPath.append(workPath).append("1_get_cplc.bat"); /*pnx_connect_jrcp.bat*/
    RunSysCmd runCmd;
    runCmd.callBatScript(batPath);

    QString cplcResultPath;
    cplcResultPath.append(workPath).append("logCplcSuccess.txt");
    fileWatcher->connect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(batGetCplcSuccess()));
    //fileWatcher->connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(batOperateSuccess(QString)));
    fileWatcher->addPath(workPath);
    //qDebug() << "start file watcher:" << workPath;
}

/*
void Nfc::batOperateSuccess(const QString &resultPath) {
    qDebug() << "batOperateSuccess:" << resultPath;
    if (resultPath.isEmpty()) {
        return;
    }
    if (resultPath.contains("logCplcSuccess")) {
        batGetCplcSuccess();
        return;
    }
    if (resultPath.contains("logAuthKeySuccess")) {
        batAuthKeySuccess();
        return;
    }
}
*/

/**
 * @brief Nfc::batGetCplcSuccess
 * bat operate get cplc success.
 */
void Nfc::batGetCplcSuccess() {
    QString cplcFailTip = QString::fromUtf8("CPLC获取失败，请重试");
    QString cplcResultPath;
    cplcResultPath.append(jcshellDirPath).append("/batForTools/logCplcSuccess.txt");
    QFile cplcResultFile(cplcResultPath);
    //qDebug() << "batGetCplcSuccess exist:" << cplcResultFile.exists();
    if (cplcResultFile.exists()) {
        QString logFilePath;
        logFilePath.append(jcshellDirPath).append("/batForTools/log.txt");
        QFile logFile(logFilePath);
        if (!logFile.exists()) {
            showLog(cplcFailTip);
            showBusy(false);
            logFile.close();
            cplcResultFile.remove();
            cplcResultFile.close();
            return;
        }
        // read log file data.
        bool open = logFile.open(QIODevice::ReadWrite | QIODevice::Text);
        if (!open) {
            showLog(cplcFailTip);
            showBusy(false);
            logFile.close();
            cplcResultFile.remove();
            cplcResultFile.close();
            return;
        }

        QString lineData;
        cplc.clear();
        QRegExp re("\\[Send\\]0x9F7F2A([a-zA-Z0-9]{84})9000");
        while (!logFile.atEnd()) {
            lineData = QString::fromUtf8(logFile.readLine());
            //qDebug() << "lineData:" << lineData;
            if (lineData.contains(re)) {
                cplc = re.cap(1);
                //qDebug() << "----lineData:" << lineData << " ,cplc:" << cplc;
                break;
            }
        }

        if (cplc.isEmpty()) {
            showLog(cplcFailTip);
        } else {
            showLog(QString::fromUtf8("CPLC:") + cplc);
        }
        showBusy(false);
        lineData.clear();
        logFile.close();
        cplcResultFile.remove();
        cplcResultFile.close();
    } /*else {
        showLog(QString::fromUtf8("文件删除").append(cplcResultPath));
    }*/
}

/**
 * @brief Nfc::jcshellBatPathBtnClick
 */
void Nfc::jcshellBatPathBtnClick() {
    jcshellBatPath = QFileDialog::getOpenFileName(nullptr, nullptr, QString("D:\\"), "(*.bat)");
    jcshellBatPathEdit->setText(jcshellBatPath);
    if (jcshellBatPath.isEmpty()) {
        showLog(QString::fromUtf8("请重新选择Jcshell脚本"));
        jcshellBatPath.clear();
        jcshellDirPath.clear();
        return;
    }
    QFile jcshellFile(jcshellBatPath);
    if (!jcshellFile.exists() || !jcshellFile.fileName().contains("jcshell")) {
        showLog(QString::fromUtf8("请选择正确的Jcshell脚本"));
        jcshellBatPath.clear();
        jcshellDirPath.clear();
        jcshellFile.close();
        return;
    }
    FileUtil fileUtil;
    jcshellDirPath = QDir::toNativeSeparators(fileUtil.getFileDir(jcshellBatPath.toStdString())).replace("\\", "/");

    QString destBatFilePath;
    destBatFilePath.append(jcshellDirPath).append("/batForTools/");
    QString destJcshellBatPath;
    destJcshellBatPath.append(jcshellDirPath).append("/scripts/");
    // qDebug() << "currentPath:" << QDir::currentPath();
    QString srcBatFilePath;
    srcBatFilePath.append(QDir::toNativeSeparators(QDir::currentPath()).replace("\\", "/")).append("/bat/");
    QString srcJcshellBatPath;
    srcJcshellBatPath.append(QDir::toNativeSeparators(QDir::currentPath()).replace("\\", "/")).append("/script/");

    QDir destBatDir(destBatFilePath);
    QDir destJcshellBatDir(destJcshellBatPath);
    //int destBatSize = destBatDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot).size();
    //int destJcshellBatSize = destJcshellBatDir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot).size();
    //qDebug() << "bat exit:" << destBatDir.exists() << " ,jcshell  exist:" << destJcshellBatDir.exists() << " ,bat size:" << destBatSize << " ,jcshell size:" << destJcshellBatSize;

    //if (destBatSize < 11 || destJcshellBatSize < 5) {
        if (cpBatFileThread) {
            QObject::disconnect(cpBatFileThread, SIGNAL(copySuccess()), this, SLOT(copyFileSuccess()));
            //qDebug() << "检查是否发送了信号槽。";
            delete cpBatFileThread;
            cpBatFileThread = nullptr;
        }
        //qDebug() << "srcBatFilePath:" << srcBatFilePath << ",destBatFilePath:" << destBatFilePath;
        cpBatFileThread = new CpFilethread(srcBatFilePath, destBatFilePath, true);
        QObject::connect(cpBatFileThread, SIGNAL(copySuccess()), this, SLOT(copyFileSuccess()));
        cpBatFileThread->start();

        if (cpJcshellThread) {
            QObject::disconnect(cpJcshellThread, SIGNAL(copySuccess()), this, SLOT(copyFileSuccess()));
            //qDebug() << "检查是否发送了信号槽。";
            delete cpJcshellThread;
            cpJcshellThread = nullptr;
        }
        //qDebug() << "srcJcshellBatPath:" << srcJcshellBatPath << ",destJcshellBatPath:" << destJcshellBatPath;
        cpJcshellThread = new CpFilethread(srcJcshellBatPath, destJcshellBatPath, true);
        QObject::connect(cpJcshellThread, SIGNAL(copySuccess()), this, SLOT(copyFileSuccess()));
        cpJcshellThread->start();

        showLog(QString::fromUtf8("正在配置环境，请稍后"));
    //}
    jcshellFile.close();
}

/**
 * @brief Nfc::checkJcshell
 * check jcshell bat exist
 * @return true: exist
 *         false: not exist.
 */
bool Nfc::checkJcshell() {
    return !jcshellBatPath.isEmpty();
}

/**
 * @brief Nfc::copyFileSuccess
 * thread copy file success slot
 */
void Nfc::copyFileSuccess() {
    copyFileTimes++;
    if (copyFileTimes >= 2) {
        showLog(QString::fromUtf8("环境准备完成。"));
        copyFileTimes = 0;
    }
}

/**
 * @brief Nfc::serverAuthKeyBtnClick
 */
void Nfc::serverAuthKeyBtnClick() {
    serverAuthKeyPath = QFileDialog::getOpenFileName(nullptr, nullptr, QString("D:\\"), "(*.txt)");
    serverAuthKeyPathEdit->setText(serverAuthKeyPath);
    QFile serverAuthKeyFile(serverAuthKeyPath);
    if (!serverAuthKeyFile.exists()) {
        showLog(QString::fromUtf8("密钥文件不存在，请检查"));
        serverAuthKeyPath.clear();
        serverAuthKeyFile.close();
        return;
    }
    serverAuthKeyFile.close();
    //qDebug() << "select server auth key path:" << serverAuthKeyPath;
}

/**
 * @brief Nfc::checkServerAuthKeyPath
 * @return true: exit
 *         false: not exit
 */
bool Nfc::checkServerAuthKeyPath() {
    return !serverAuthKeyPath.isEmpty();
}

/**
 * @brief Nfc::writeAuthKeyBtnClick
 */
void Nfc::writeAuthKeyBtnClick() {
    if (!checkJcshell()) {
        showLog(QString::fromUtf8("请先设置Jcshell脚本路径"));
        return;
    }
    if (!checkServerAuthKeyPath()) {
        showLog(QString::fromUtf8("请先导入密钥文件"));
        return;
    }

    if (cplc.isEmpty()) {
        getCplcBtnClick();
        showLog(QString::fromUtf8("获取CPLC成功后，需要再次点击“写入密钥”按钮"));
        return;
    }

    // read file data.
    QFile serverAuthKeyFile(serverAuthKeyPath);
    if (!serverAuthKeyFile.exists()) {
        showLog(QString::fromUtf8("密钥文件不存在，请检查"));
        serverAuthKeyFile.close();
        return;
    }
    bool open = serverAuthKeyFile.open(QIODevice::ReadWrite | QIODevice::Text);
    if (!open) {
        showLog(QString::fromUtf8("密钥文件打开失败，请检查"));
        serverAuthKeyFile.close();
        return;
    }
    QString lineData;
    QString enc;
    QString mac;
    QString dek;
    QRegExp reEnc("^enc:([a-zA-Z0-9]{32})");
    QRegExp reMac("^mac:([a-zA-Z0-9]{32})");
    QRegExp reDek("^dek:([a-zA-Z0-9]{32})");
    showLog(QString::fromUtf8("正在获取密钥"));
    showBusy(true);
    while (!serverAuthKeyFile.atEnd()) {
        lineData = QString::fromUtf8(serverAuthKeyFile.readLine());
        //qDebug() << "lineData:" << lineData;
        if (lineData.indexOf(cplc) != -1) {
            enc = QString::fromUtf8(serverAuthKeyFile.readLine());
            mac = QString::fromUtf8(serverAuthKeyFile.readLine());
            dek = QString::fromUtf8(serverAuthKeyFile.readLine());
            if (enc.indexOf(reEnc) != -1) {
                enc = reEnc.cap(1);
            }
            if (mac.indexOf(reMac) != -1) {
                mac = reMac.cap(1);
            }
            if (dek.indexOf(reDek) != -1) {
                dek = reDek.cap(1);
            }
            //qDebug() << "enc:" << enc;
            //qDebug() << "mac:" << mac;
            //qDebug() << "dek:" << dek;
            break;
        }
    }

    if (enc.isEmpty() || mac.isEmpty() || dek.isEmpty()) {
        showLog(QString::fromUtf8("密钥提取失败，请检查密钥文件"));
        showBusy(false);
        serverAuthKeyFile.close();
        return;
    }

    showLog(QString::fromUtf8("密钥为："));
    showLog(enc);
    showLog(mac);
    showLog(dek);
    serverAuthKeyFile.close();

    QString setKeyJcshellPath;
    setKeyJcshellPath.append(jcshellDirPath).append("/scripts/setkey4.jcsh");
    //qDebug() << "setKeyJcshellPath:" << setKeyJcshellPath;

    QFile setkeyJcshellFile(setKeyJcshellPath);
    open = setkeyJcshellFile.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!open) {
        showLog(QString::fromUtf8("密钥脚本打开失败，请检查setkey4.jcshell文件"));
        showBusy(false);
        setkeyJcshellFile.close();
        return;
    }

    QString key1;
    QString key2;
    QString key3;
    QRegExp reKey1("^set-key\\s33/1/DES-ECB/([a-zA-Z0-9]{32})");
    QRegExp reKey2("^set-key\\s33/2/DES-ECB/([a-zA-Z0-9]{32})");
    QRegExp reKey3("^set-key\\s33/3/DES-ECB/([a-zA-Z0-9]{32})");
    showLog(QString::fromUtf8("正在设置密钥"));
    while (!setkeyJcshellFile.atEnd()) {
        lineData = QString::fromUtf8(setkeyJcshellFile.readLine());
        //qDebug() << "setKeyJcshellFile lineData:" << lineData;
        if (lineData.indexOf(reKey1) != -1) {
            key1 = reKey1.cap(1);
            key2 = QString::fromUtf8(setkeyJcshellFile.readLine());
            if (key2.indexOf(reKey2) != -1) {
                key2 = reKey2.cap(1);
            }
            key3 = QString::fromUtf8(setkeyJcshellFile.readLine());
            if (key3.indexOf(reKey3) != -1) {
                key3 = reKey3.cap(1);
            }
        }
        //qDebug() << "key1:" << key1;
        //qDebug() << "key2:" << key2;
        //qDebug() << "key3:" << key3;

        if (!key1.isEmpty() && !key2.isEmpty() && !key3.isEmpty()) {
            break;
        }
        if (lineData.contains("i-u")) {
            break;
        }
    }

    if (key1.isEmpty() || key2.isEmpty() || key3.isEmpty()) {
        showLog(QString::fromUtf8("密钥脚本原始密钥获取失败，请检查setkey4.jcshell文件"));
        showBusy(false);
        setkeyJcshellFile.close();
        return;
    }

    // replace key and write to file
    setkeyJcshellFile.seek(0);
    QString allDataTmp(setkeyJcshellFile.readAll());
    allDataTmp.replace(allDataTmp.indexOf(key1), key1.length(), enc);
    allDataTmp.replace(allDataTmp.indexOf(key2), key2.length(), mac);
    allDataTmp.replace(allDataTmp.indexOf(key3), key3.length(), dek);
    // qDebug() << "\n allData:" << allDataTmp;
    // rewrite setkey4.jcshell file.
    // 1. close file(read mode) first.
    setkeyJcshellFile.close();
    // 2. open file(rewrite mode)
    open = setkeyJcshellFile.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    if (!open) {
        showLog(QString::fromUtf8("密钥替换失败，请检查setkey4.jcshell文件"));
        showBusy(false);
        setkeyJcshellFile.close();
        return;
    }
    setkeyJcshellFile.write(allDataTmp.toUtf8());
    setkeyJcshellFile.flush();
    setkeyJcshellFile.close();

    // start 2_auth_key.bat
    //2_auth_key.bat中的语句全为调用次级vbs脚本，再由vbs调用子bat脚本，从而达到隐藏窗口的目的
    QString workPath;
    workPath.append(jcshellDirPath).append("/batForTools/");
    QString batPath;
    batPath.append(workPath).append("2_auth_key.bat");
    RunSysCmd runCmd;
    runCmd.callBatScript(batPath);
    // file watcher
    //QString authKeyResultPath;
    //authKeyResultPath.append(workPath).append("logAuthKeySuccess.txt");
    fileWatcher->connect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(batAuthKeySuccess()));
    //fileWatcher->connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(batOperateSuccess(QString)));
    fileWatcher->addPath(workPath);
}

/**
 * @brief Nfc::batAuthKeySuccess
 * bat operate auth key success.
 */
void Nfc::batAuthKeySuccess() {
    QString authKeyFailTip = QString::fromUtf8("密钥写入失败，请重试");
    QString authKeyResultPath;
    authKeyResultPath.append(jcshellDirPath).append("/batForTools/logAuthKeySuccess.txt");
    QFile authKeyResultFile(authKeyResultPath);
    //qDebug() << "batAuthKeySuccess exist:" << authKeyResultFile.exists();
    if (authKeyResultFile.exists()) {
        QString logFilePath;
        logFilePath.append(jcshellDirPath).append("/batForTools/log.txt");
        QFile logFile(logFilePath);
        if (!logFile.exists()) {
            showLog(authKeyFailTip);
            showBusy(false);
            logFile.close();
            authKeyResultFile.remove();
            authKeyResultFile.close();
            return;
        }
        // read log file data.
        bool open = logFile.open(QIODevice::ReadWrite | QIODevice::Text);
        if (!open) {
            showLog(authKeyFailTip);
            showBusy(false);
            logFile.close();
            authKeyResultFile.remove();
            authKeyResultFile.close();
            return;
        }

        QString allDataTmp(logFile.readAll());
        // put-keyset 33 success
        if (allDataTmp.contains("0x84D821814B2180") && allDataTmp.contains("0x218BAF478BAF478BAF479000")) {
            showLog("测试密钥写入成功");
        } else {
            showLog("测试密钥写入失败");
        }

        showBusy(false);
        logFile.close();
        authKeyResultFile.remove();
        authKeyResultFile.close();
    }
}

/**
 * @brief Nfc::checkAuthKeyBtnClick
 */
void Nfc::checkAuthKeyBtnClick() {
    if (!checkJcshell()) {
        showLog(QString::fromUtf8("请先设置Jcshell脚本路径"));
        return;
    }
    // we delete log.txt first, because jchsell check command is the same.
    QString logFilePath;
    logFilePath.append(jcshellDirPath).append("/batForTools/log.txt");
    QFile logFile(logFilePath);
    logFile.remove();
    logFile.close();

    //3_check_key.bat中的语句全为调用次级vbs脚本，再由vbs调用子bat脚本，从而达到隐藏窗口的目的
    showLog(QString::fromUtf8("正在校验密钥"));
    showBusy(true);
    QString workPath;
    workPath.append(jcshellDirPath).append("/batForTools/");
    QString batPath;
    batPath.append(workPath).append("3_check_key.bat");
    RunSysCmd runCmd;
    runCmd.callBatScript(batPath);
    // file watcher
    //QString checkKeyResultPath;
    //checkKeyResultPath.append(workPath).append("logCheckKeySuccess.txt");
    fileWatcher->connect(fileWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(batCheckKeySuccess()));
    //fileWatcher->connect(fileWatcher, SIGNAL(fileChanged(QString)), this, SLOT(batOperateSuccess(QString)));
    fileWatcher->addPath(workPath);
}

void Nfc::batCheckKeySuccess() {
    // TODO check key 之后需要delete log.txt  cplc.clear();
    QString checkKeyFailTip = QString::fromUtf8("校验密钥失败，请重试");
    QString checkKeyResultPath;
    checkKeyResultPath.append(jcshellDirPath).append("/batForTools/logCheckKeySuccess.txt");
    QFile checkKeyResultFile(checkKeyResultPath);
    //qDebug() << "batCheckKeySuccess exist:" << checkKeyResultFile.exists();
    if (checkKeyResultFile.exists()) {
        QString logFilePath;
        logFilePath.append(jcshellDirPath).append("/batForTools/log.txt");
        QFile logFile(logFilePath);
        if (!logFile.exists()) {
            showLog(checkKeyFailTip);
            showBusy(false);
            logFile.close();
            checkKeyResultFile.remove();
            checkKeyResultFile.close();
            return;
        }
        // read log file data.
        bool open = logFile.open(QIODevice::ReadWrite | QIODevice::Text);
        if (!open) {
            showLog(checkKeyFailTip);
            showBusy(false);
            logFile.close();
            checkKeyResultFile.remove();
            checkKeyResultFile.close();
            return;
        }

        bool checkStatus = false;
        QRegExp reIU("\\[Recv\\]0x8050210008.*");
        QRegExp reEAMac("\\[Recv\\]0x84820100.*");
        QRegExp successFlag(".*9000");
        QString lineData;
        while(!logFile.atEnd()) {
            lineData = QString::fromUtf8(logFile.readLine());
            if (lineData.indexOf(reIU) != -1) {
                lineData = QString::fromUtf8(logFile.readLine());
                if (!lineData.contains(successFlag)) {
                    checkStatus = false;
                    break;
                }
                lineData = QString::fromUtf8(logFile.readLine());
                if (lineData.indexOf(reEAMac) != -1) {
                    lineData = QString::fromUtf8(logFile.readLine());
                    if (lineData.contains(successFlag)) {
                        checkStatus = true;
                        break;
                    }
                } else {
                    checkStatus = false;
                    break;
                }
            }
        }

        if (checkStatus) {
            showLog("密钥校验完成，测试密钥写入成功");
        } else {
            showLog("测试密钥校验或写入失败，请检查");
        }

        showBusy(false);
        cplc.clear();
        logFile.remove();
        logFile.close();
        checkKeyResultFile.remove();
        checkKeyResultFile.close();
    }
}

void Nfc::showBusy(const bool &busy) {
    if (!busy) {
        progressBar->setVisible(false);
    } else {
        progressBar->setOrientation(Qt::Horizontal);
        progressBar->setMinimum(0);
        progressBar->setMaximum(0);
        progressBar->setVisible(true);
    }
}

Nfc::~Nfc() {
    if (cpBatFileThread) {
        delete cpBatFileThread;
        cpBatFileThread= nullptr;
    }
    if (cpJcshellThread) {
        delete cpJcshellThread;
        cpJcshellThread= nullptr;
    }
    delete progressBar;
    delete fileWatcher;
    delete jcshellBatPathBtn;
    delete jcshellBatPathEdit;
    delete serverAuthKeyPathBtn;
    delete serverAuthKeyPathEdit;
    delete logEdit;
    delete getCplcBtn;
    delete writeAuthKeyBtn;
    delete checkAuthKeyBtn;
    delete serverAuthKeyPathLayout;
    delete jcshellBatPathLayout;
    delete operBtnsLayout;
    delete mainLayout;
}
