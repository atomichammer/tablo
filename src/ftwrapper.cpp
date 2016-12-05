#include "ftwrapper.h"
#include <QDebug>
#include <QApplication>
FTWrapper::FTWrapper(QObject *parent) :
    QObject(parent)
{
    lib.setFileName("ftd2xx.dll");
    success = false;
    if(lib.load())
    {
        success = true;
        FTListDevices = (_FT_ListDevices)lib.resolve("FT_ListDevices");
        FTGetLibraryVersion = (_FT_GetLibraryVersion)lib.resolve("FT_GetLibraryVersion");
        FTGetComPortNumber = (_FT_GetComPortNumber)lib.resolve("FT_GetComPortNumber");
        FTOpenEx = (_FT_OpenEx)lib.resolve("FT_OpenEx");
        FTClose = (_FT_Close)lib.resolve("FT_Close");
    }
}

QList<PORT> FTWrapper::getPorts()
{
    return ports;
}

void FTWrapper::scanPorts()
{
    DWORD devIndex = 0;
    char desc[64];
    DWORD num;
    PORT port;

    ports.clear();

    FT_STATUS ftStatus = FTListDevices(&num, 0, FT_LIST_NUMBER_ONLY);
    if (ftStatus == FT_OK) //Get first device serial number
    {
            qDebug() << "Number of Devices: " << num;

            for(uint i = 0; i < num; ++i)
            {
                devIndex = i;
                ftStatus = ~FT_OK;
                int k = 0;
                while(ftStatus != FT_OK)
                {
                    ftStatus = FTListDevices((PVOID)devIndex, desc, FT_LIST_BY_INDEX | FT_OPEN_BY_DESCRIPTION);
                    QApplication::processEvents();
                    qDebug() << k++;
                }
                /*
                if (ftStatus != FT_OK) //Get first device serial number
                {
                        qDebug() << devIndex <<  " Couldn't get FTDI device description " << ftStatus;
                        continue;
                }
                */

                FT_HANDLE ftHandle; // valid handle returned from FT_OpenEx
                LONG lComPortNumber;

                ftStatus = FTOpenEx((PVOID)desc, FT_OPEN_BY_DESCRIPTION, &ftHandle);

                if(ftStatus == FT_OK)
                {
                    ftStatus = FTGetComPortNumber(ftHandle,&lComPortNumber);
                    if (ftStatus == FT_OK)
                    {
                        if (lComPortNumber == -1)
                        {

                        }else
                        {
                            port.description = QString::fromStdString(desc);
                            port.name = QString("COM%1").arg(lComPortNumber);
                            ports.append(port);
                        }
                    }
                    else
                    {

                    }
                    FTClose(ftHandle);
                }
            }
    }  else qDebug() << "Couldn't get FTDI devices number";
    qDebug() << "ScanEnd";
}
