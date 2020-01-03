#include "msgthread.h"

MsgThread::MsgThread()
{

}

void MsgThread::run()
{
    while(true)
    {
        qDebug() << "hello";
        QString message = "Hi";
       emit updateMsg(message);
    }
}
