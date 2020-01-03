#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{
    m_nProcessType = CMD_TYPE_INIT;
}

void Worker::runProcess(sProcessARG& arg)
{
    Process process;
    connect(&process,SIGNAL(sendMsg(QString)),this,SLOT(recvMsg(QString)));

    switch (arg.nType)
    {
    case CMD_TYPE_COMPILE:
        ApktoolBuild(process, arg);
        break;
    case CMD_TYPE_DECOMPILE:
        ApktoolDecode(process, arg);
        break;
    case CMD_TYPE_KILL:
        qDebug() << "kill";
        process.killProcess();
        break;
    case CMD_TYPE_ACT_STACK:
        ActStackDump(process, arg);
        break;
    case CMD_TYPE_APK_INFO:
        ApkInfoDump(process, arg);
        break;
    default:
        m_nProcessType = CMD_TYPE_INIT;
        break;
    }

    QString msg = "------------------------------------------------";
    emit updateMsg(m_nProcessType, msg);
}

void Worker::ApktoolDecode(Process& process, sProcessARG& arg)
{
    m_nProcessType = CMD_TYPE_DECOMPILE;
    QString cmd = "apktool -f d " + arg.strInputPath + " -o " + arg.strOutputPath;
    emit updateMsg(m_nProcessType, "CMD: " + cmd);
    process.runProcess(arg.strEnvPath + cmd);
}

void Worker::ApktoolBuild(Process& process, sProcessARG& arg)
{
    m_nProcessType = CMD_TYPE_COMPILE;
    QString cmd = "apktool -d b " + arg.strInputPath;
    emit updateMsg(m_nProcessType, "CMD: " + cmd);
    process.runProcess(arg.strEnvPath + cmd);
}

void Worker::ActStackDump(Process& process, sProcessARG& arg)
{
    m_nProcessType = CMD_TYPE_ACT_STACK;
    QString cmd =  "adb devices";
    emit updateMsg(m_nProcessType, "CMD: " + cmd);
    process.runProcess(arg.strEnvPath + cmd);
}

void Worker::ApkInfoDump(Process &process, sProcessARG &arg)
{
    m_nProcessType = CMD_TYPE_APK_INFO;
    QString cmd =  "aapt dump badging " + arg.strInputPath;
    emit updateMsg(m_nProcessType, "CMD: " + cmd);
    process.runProcess(arg.strEnvPath + cmd);
}

void Worker::recvMsg(QString msg)
{
    switch (m_nProcessType)
    {
    case CMD_TYPE_APK_INFO:
    {
        QString pattern = "package: name=(?<packageName>.*) v.*versionName=(?<versionName>.*) .*"
                          "sdkVersion:(?<sdkVersion>.*) .*";
        m_reExp.setPattern(pattern);
        m_reExp.setPatternOptions(QRegularExpression::InvertedGreedinessOption);
        m_expMatch = m_reExp.match(msg);

        if(m_expMatch.hasMatch())
        {
            QString packageName = m_expMatch.captured("packageName");
            QString versionName = m_expMatch.captured("versionName");
            QString sdkVersion = m_expMatch.captured("sdkVersion");

            emit updateMsg(m_nProcessType, "Apk Name   : " + packageName);
            emit updateMsg(m_nProcessType, "Apk Version: " + versionName);
            emit updateMsg(m_nProcessType, "Sdk Version: " + sdkVersion);
        }
        break;
    }
    default:
        emit updateMsg(m_nProcessType, msg);
        break;
    }
}
