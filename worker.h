#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "process.h"

#define CMD_TYPE_INIT       0x00
#define CMD_TYPE_KILL       0x01
#define CMD_TYPE_DECOMPILE  0x02
#define CMD_TYPE_COMPILE    0x03
#define CMD_TYPE_ACT_STACK   0x04
#define CMD_TYPE_UPDATA_PATH 0x05
#define CMD_TYPE_APK_INFO   0x06

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

signals:
    void updateMsg(int nType, QString msg);

public slots:
    void runProcess(sProcessARG& arg);
    void recvMsg(QString msg);

private:
    int m_nProcessType;
    sPATH m_sPath;
    QRegularExpression m_reExp;
    QRegularExpressionMatch m_expMatch;

    void ApktoolDecode(Process& process, sProcessARG &arg);
    void ApktoolBuild(Process& process, sProcessARG& arg);
    void ActStackDump(Process& process, sProcessARG& arg);
    void ApkInfoDump(Process& process, sProcessARG& arg);
};

#endif // WORKER_H
