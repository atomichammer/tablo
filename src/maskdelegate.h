#ifndef MASKDELEGATE_H
#define MASKDELEGATE_H

#include <QItemDelegate>

class MaskDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit MaskDelegate(QWidget *parent = 0);
    
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex & index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &) const;

signals:
    //void returnPressed();
public slots:
    void retPressed();
};

#endif // MASKDELEGATE_H
