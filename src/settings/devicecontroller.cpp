#include "devicecontroller.h"
#include <QDebug>

DeviceController::DeviceController(DeviceModel *_devices)
{
    devices = _devices;
}

DeviceController::~DeviceController()
{
}

QModelIndex DeviceController::addDevice(bool enabled, int address, QString name,
                                 int type, int status, QString port)
{
//   0 bool enabled;
//   1 unsigned char address;
//   2 unsigned char type;
//   3 QString name;
//   4 unsigned char status;
//   5 QString port;
    QModelIndex index;

    devices->appendRow();

    int row = 0;
    if(devices->rowCount(QModelIndex()) > 0)
    {
        row = devices->rowCount(QModelIndex()) - 1;
    }

    index = devices->index(row, 0);


    setEnabled(&index, enabled);
    setAddress(&index, address);
    setType(&index, type);
    setName(&index, name);
    setStatus(&index, status);
    setPort(&index, port);

    return index;
}

void DeviceController::setEnabled(QModelIndex *index, bool enabled)
{
    QModelIndex idx = devices->index(index->row(), 0);
    if(idx.isValid())
    {
        if(enabled)
            devices->setData(idx, Qt::Checked, Qt::CheckStateRole);
        else
            devices->setData(idx, Qt::Unchecked, Qt::CheckStateRole);
    }
}

void DeviceController::setAddress(QModelIndex *index, int address)
{
    QModelIndex idx = devices->index(index->row(), 1);
    if(idx.isValid())
    {
        devices->setData(idx, address, Qt::DisplayRole);
    }
}

void DeviceController::setType(QModelIndex *index, int type)
{
    QModelIndex idx = devices->index(index->row(), 2);
    if(idx.isValid())
    {
        devices->setData(idx, type, Qt::DisplayRole);
    }
}

void DeviceController::setName(QModelIndex *index, QString name)
{
    QModelIndex idx = devices->index(index->row(), 3);
    if(idx.isValid())
    {
        devices->setData(idx, name, Qt::DisplayRole);
    }
}

void DeviceController::setStatus(QModelIndex *index, int status)
{
    QModelIndex idx = devices->index(index->row(), 4);
    if(idx.isValid())
    {
        devices->setData(idx, status, Qt::DisplayRole);
    }
}

void DeviceController::setPort(QModelIndex *index, QString port)
{
    QModelIndex idx = devices->index(index->row(), 6);
    if(idx.isValid())
    {
        devices->setData(idx, port, Qt::DisplayRole);
    }
}

void DeviceController::setLength(QModelIndex *index, int length)
{
    QModelIndex idx = devices->index(index->row(), 5);
    if(idx.isValid())
    {
        devices->setData(idx, length, Qt::EditRole);
    }
}

void DeviceController::setSwap(QModelIndex *index, bool swap)
{
    QModelIndex idx = devices->index(index->row(), 7);
    if(idx.isValid())
    {
        devices->setData(idx, swap, Qt::EditRole);
    }
}

void DeviceController::setCurrencyList()
{

}
