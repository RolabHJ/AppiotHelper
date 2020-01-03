#ifndef MSGTHREAD_H
#define MSGTHREAD_H

#include <QThread>
#include <QDebug>

class MsgThread : public QThread
{
public:
    MsgThread();

protected:
    void run();

signals:
    void updateMsg(QString& msg);
};

#endif // MSGTHREAD_H
