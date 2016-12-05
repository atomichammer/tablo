#ifndef DeviceController_H
#define DeviceController_H

#include "settings/devicemodel.h"
#include <QStringListModel>


class DeviceController : public QObject
{
    Q_OBJECT
    
public:
    explicit DeviceController(DeviceModel *_devices);
    ~DeviceController();
    DeviceModel *devices;

    void setEnabled(QModelIndex *index, bool enabled);
    void setAddress(QModelIndex *index, int address);
    void setType(QModelIndex *index, int type);
    void setName(QModelIndex *index, QString name);
    void setStatus(QModelIndex *index, int status);
    void setPort(QModelIndex *index, QString port);
    void setLength(QModelIndex *index, int length);
    void setSwap(QModelIndex *index, bool swap);
    QModelIndex addDevice(bool enabled, int address, QString name, int type, int status, QString port);
    void setCurrencyList();
private slots:

private:


};

#endif // DeviceController_H
