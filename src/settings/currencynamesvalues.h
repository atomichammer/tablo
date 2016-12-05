#ifndef CURRENCYNAMESVALUES_H
#define CURRENCYNAMESVALUES_H

#include <QAbstractTableModel>

typedef struct
{
    int id;
    QString name;
    QString  buy;
    QString sell;
} CURR;

class CurrencyNamesValues : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CurrencyNamesValues(QObject *parent = 0);
    
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const { Q_UNUSED(parent) return 4; }
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant data(const QModelIndex &index, int role) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);
    void clear(const QModelIndex &parent);
    void appendRow( QString name, QString buy, QString sell );
    CURR getRow(int index);
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QStringList getCurrencyNamesList();
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QStringList valuesList();
signals:
    
public slots:
private:
    Qt::DropActions supportedDropActions() const;
    QList<CURR> currencyList;

};

#endif // CURRENCYNAMESVALUES_H
