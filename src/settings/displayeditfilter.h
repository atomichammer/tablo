#ifndef DISPLAYEDITFILTER_H
#define DISPLAYEDITFILTER_H

#include <QSortFilterProxyModel>

class DisplayEditFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit DisplayEditFilter(QObject *parent = 0);
    
    bool filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
signals:
    
public slots:
    
};

#endif // DISPLAYEDITFILTER_H
