#include "process.h"

Process::Process(QObject *parent) : QObject(parent)
{
    m_pProcess = new QProcess;

//    QString path = "/Users/hhj/Library/Android/sdk/platform-tools:/usr/local/bin/:" + qgetenv("PATH");
//    qputenv("PATH", path.toStdString().c_str());

    connect(m_pProcess,SIGNAL(readyReadStandardError()),this,SLOT(readErr()));
    connect(m_pProcess,SIGNAL(readyReadStandardOutput()),this,SLOT(readOut()));
}

void Process::runProcess(QString cmd)
{
    m_pProcess->start(cmd);
    m_pProcess->waitForFinished();
}

void Process::killProcess()
{
    if(m_pProcess != nullptr)
        m_pProcess->kill();
}

void Process::readOut()
{
    QString str = m_pProcess->readAllStandardOutput().simplified();
    emit sendMsg(str);
    qDebug() << str;
}

void Process::readErr()
{
    QString str = m_pProcess->readAllStandardError().simplified();
    emit sendMsg(str);
    qDebug() << str;
}
