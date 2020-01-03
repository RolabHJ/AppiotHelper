#ifndef PROCESS_H
#define PROCESS_H

#include <QObject>
#include <QProcess>
#include <QDebug>

struct sProcessARG{
    QString strInputPath;
    QString strOutputPath;
    QString strEnvPath;
    QString strArg;
    int nType;
};

struct sPATH{
    QString inRootPath;
    QString outRootPath;
    QString apktoolPath;
    QString apkPath;
    QString smaliPath;
    QString dexPath;
};

struct sAPKINFO{
    QString strPackageName;
    QString strApkVersion;
};

class Process : public QObject
{
    Q_OBJECT
public:
    explicit Process(QObject *parent = nullptr);
    void runProcess(QString cmd);
    void killProcess();

signals:
    void sendMsg(QString msg);

public slots:
    void readOut();
    void readErr();

private:
    QProcess* m_pProcess;
};

#endif // PROCESS_H
