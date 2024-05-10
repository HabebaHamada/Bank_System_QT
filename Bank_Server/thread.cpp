#include "thread.h"

Thread::Thread(qintptr socketDescriptor,QObject *parent)
    : QObject{parent},socketDescriptor(socketDescriptor)
{}
