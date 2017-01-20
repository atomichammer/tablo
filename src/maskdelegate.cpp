#include "maskdelegate.h"
#include "mylineedit.h"
#include <QDebug>


MaskDelegate::MaskDelegate(QWidget *parent) :
    QItemDelegate(parent)
{
}

QWidget *MaskDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex & index) const
{

    MyLineEdit *editor = new MyLineEdit(parent);

    editor->setIndex(index.row());
    editor->setAlignment(Qt::AlignCenter);
    connect(editor, SIGNAL(returnPressed()), this, SLOT(retPressed()));
    return editor;
}

void MaskDelegate::retPressed()
{
    //emit returnPressed();
}

void MaskDelegate::setEditorData(QWidget *editor,
                                    const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    MyLineEdit *lineEdit = static_cast<MyLineEdit*>(editor);
    lineEdit->setText(value);
}

void MaskDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    MyLineEdit *lineEdit = static_cast<MyLineEdit*>(editor);

    QString value = lineEdit->text();
    model->setData(index, value, Qt::EditRole);
}

void MaskDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}
