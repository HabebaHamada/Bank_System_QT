#ifndef THREAD_H
#define THREAD_H

#include <QObject>
#include <QTcpSocket>

class Thread : public QObject
{
    Q_OBJECT
public:
    explicit Thread(qintptr socketDescriptor,QObject *parent = nullptr);

private:
    qintptr socketDescriptor;
};

#endif // THREAD_H
