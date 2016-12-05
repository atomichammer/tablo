#include <QApplication>
#include <QComboBox>
#include "cbdelegate.h"
#include <QDebug>
#include <QModelIndex>

cbDelegate::cbDelegate(QObject *parent)
:QItemDelegate(parent)
{
}

QWidget *cbDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox* editor = new QComboBox(parent);

    editor->setModel(namesModel);
    editor->setModelColumn(1);
    return editor;
}

void cbDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);

  int value = index.model()->data(index, Qt::DisplayRole).toUInt();
  comboBox->setCurrentIndex(value);
}

void cbDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
  QComboBox *comboBox = static_cast<QComboBox*>(editor);
  model->setData(index, comboBox->currentIndex(), Qt::EditRole);
}

void cbDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
  editor->setGeometry(option.rect);
}

void cbDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

  QStyleOptionViewItemV4 myOption = option;
  QModelIndex index_0;

  index_0 = index.model()->index(index.row(), 0, QModelIndex());
  int val = index.model()->data(index_0, Qt::DisplayRole).toInt();
  index_0 = namesModel->index(val, 1, QModelIndex());
  QString text = namesModel->data(index_0, Qt::DisplayRole).toString();

  myOption.text = text;

  QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &myOption, painter);

}
