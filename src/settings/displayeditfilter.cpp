#include "displayeditfilter.h"
#include <QColor>
DisplayEditFilter::DisplayEditFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool DisplayEditFilter::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    switch(source_column)
    {
        case 0:
        case 1: return false;
        case 2:
        case 3: return true;
        default: return false;
    }
}

QVariant DisplayEditFilter::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch(section)
        {
            case 0: return QString(tr("Buy"));
            case 1: return QString(tr("Sell"));
            default: return QVariant();
        }
    }

    if (orientation == Qt::Vertical)
    {
        QModelIndex idx = sourceModel()->index(section, 1, QModelIndex());
        return QVariant(sourceModel()->data(idx, Qt::DisplayRole).toString());
    }

    return QVariant();
}

QVariant DisplayEditFilter::data( const QModelIndex & index, int role ) const
{
    if (!index.isValid())
      return QVariant();

    QModelIndex sourceIndex = mapToSource(index);
    if(role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }

    return sourceModel()->data(sourceIndex, role);
}
