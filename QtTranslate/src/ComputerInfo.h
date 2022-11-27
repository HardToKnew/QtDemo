#ifndef COMPUTERINFO_H
#define COMPUTERINFO_H
#include <QDebug>
#include <QProcess>
#include <windows.h>

class ComputerInfo
{
public:
    ComputerInfo();
    QString getCpuName();
    QString getCpuProcessorId();
    QString getBaseboardSerialNum();
    QString getBiosSerialNum();
    QString getDiskSerialNum(int index);
};

#endif // COMPUTERINFO_H


