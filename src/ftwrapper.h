#ifndef FTWRAPPER_H
#define FTWRAPPER_H

#include <QObject>
#include <QLibrary>
#include <QList>
#include <windows.h>

extern "C"{
#include "ftd2xx.h"
}

typedef FT_STATUS (__stdcall *_FT_ListDevices)(PVOID pArg1, PVOID pArg2, DWORD Flags);
typedef FT_STATUS (__stdcall *_FT_GetLibraryVersion)(LPDWORD lpdwDLLVersion);
typedef FT_STATUS (__stdcall *_FT_GetComPortNumber)(FT_HANDLE ftHandle, LPLONG lpdwComPortNumber);
typedef FT_STATUS (__stdcall *_FT_OpenEx)(PVOID pArg1, DWORD Flags, FT_HANDLE *pHandle);
typedef FT_STATUS (__stdcall *_FT_Close)(FT_HANDLE ftHandle);

typedef struct{
    QString description;
    QString name;
} PORT;

class FTWrapper : public QObject
{
    Q_OBJECT
public:
    explicit FTWrapper(QObject *parent = 0);
    _FT_ListDevices FTListDevices;
    _FT_GetLibraryVersion FTGetLibraryVersion;
    _FT_GetComPortNumber FTGetComPortNumber;
    _FT_OpenEx FTOpenEx;
    _FT_Close FTClose;
    bool isSuccessful() { return success; }
    
    void scanPorts();
    QList<PORT> getPorts();
signals:
    
public slots:
private:
    QLibrary lib;
    QList<PORT> ports;
    bool success;
    
};

#endif // FTWRAPPER_H
