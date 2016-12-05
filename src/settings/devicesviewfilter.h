#ifndef DEVICESVIEWFILTER_H
#define DEVICESVIEWFILTER_H

#include <QSortFilterProxyModel>

class DevicesViewFilter : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    DevicesViewFilter();

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;

    //QVariant data(const QModelIndex &index, int role) const;
private:

};
#endif // DEVICESVIEWFILTER_H
