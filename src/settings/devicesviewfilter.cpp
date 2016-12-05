#include "DevicesViewFilter.h"
#include <QDebug>
#include <QColor>

DevicesViewFilter::DevicesViewFilter()
{

}

bool DevicesViewFilter::filterAcceptsRow(int sourceRow,
        const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceRow);
    Q_UNUSED(sourceParent);
    return true;
}

bool DevicesViewFilter::filterAcceptsColumn(int source_column, const QModelIndex &source_parent) const
{
    Q_UNUSED(source_parent);
    return true;
    switch(source_column)
    {
        case 0: return false;
        case 1:
        case 2:
        case 3: return true;
        default: return false;
    }
}
