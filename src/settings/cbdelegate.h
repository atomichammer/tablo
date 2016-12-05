#ifndef CBDELEGATE_H
#define CBDELEGATE_H

#include <QItemDelegate>
#include "settings/currencynamesvalues.h"
#include <string>
#include <vector>

class QModelIndex;
class QWidget;
class QVariant;

class cbDelegate : public QItemDelegate
{
Q_OBJECT
public:
  cbDelegate(QObject *parent = 0);

  QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setEditorData(QWidget *editor, const QModelIndex &index) const;
  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
  void setCurrencyList(CurrencyNamesValues *NamesModel) { namesModel = NamesModel; }

private:
  CurrencyNamesValues *namesModel;
  std::vector<std::string> Items;

};

#endif // CBDELEGATE_H
