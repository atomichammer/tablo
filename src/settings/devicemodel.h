#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractTableModel>
#include <QStandardItemModel>
#include <QStringListModel>

typedef struct
{
        bool enabled;
        unsigned char address;
        unsigned char type;
        QString name;
        unsigned char status;
        int length;
        QString portName;
        QStandardItemModel *currConfig;
        bool swap;
} DEVICE;

class DeviceModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    DeviceModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const {  Q_UNUSED(parent) return 8; }
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
    bool insertRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    bool removeRows ( int row, int count, const QModelIndex & parent = QModelIndex() );
    void appendRow();
    void clear(const QModelIndex & parent);
    QList<DEVICE> getList();
    void setList(QList<DEVICE> l) { list = l; /*reset();*/ }
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    DEVICE getDevice(int row);
    void setCurrencyNames(QStringListModel *model) { currencyNames = model; }
    void setCurrencyRows( int rows ) { currRows = rows; }
signals:
    void changed();
private:
     QList <DEVICE>list;
     Qt::ItemFlags flags(const QModelIndex &index) const;
     QStringListModel *currencyNames;
     enum STATE { OFFLINE, DAY, NIGHT, OFF };
     int currRows;

};

#endif // DEVICEMODEL_H
