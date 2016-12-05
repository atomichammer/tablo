#include "currencynamesdialog.h"

#include <QDebug>

CurrencyNamesDialog::CurrencyNamesDialog(CurrencyNamesValues *_model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CurrencyNamesDialog)
{
    ui->setupUi(this);
    model = _model;
    ui->lvCurrencyNames->setModel(model);
    ui->lvCurrencyNames->setModelColumn(1);
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    ui->lvCurrencyNames->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->lvCurrencyNames->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->lvCurrencyNames->setDragEnabled(true);
    ui->lvCurrencyNames->viewport()->setAcceptDrops(true);
    ui->lvCurrencyNames->setDefaultDropAction(Qt::MoveAction);
    ui->lvCurrencyNames->setDropIndicatorShown(true);
    ui->lvCurrencyNames->setMovement(QListView::Snap);
    ui->lvCurrencyNames->setDragDropOverwriteMode(false);
    ui->lvCurrencyNames->setDragDropMode(QAbstractItemView::InternalMove);
    //
    ui->lbTotal->setText(QString(tr("Total: %1").arg(model->rowCount(QModelIndex()))));
    //
//    ui->tableView->setModel(model);
//    ui->tableView->hideColumn(2);
//    ui->tableView->hideColumn(3);

//    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
//    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
//    ui->tableView->setDragEnabled(true);
//    ui->tableView->viewport()->setAcceptDrops(true);
//    ui->tableView->setDefaultDropAction(Qt::MoveAction);
//    ui->tableView->setDropIndicatorShown(true);
//    ui->tableView->setDragDropOverwriteMode(false);
//    ui->tableView->setDragDropMode(QAbstractItemView::InternalMove);

}

CurrencyNamesDialog::~CurrencyNamesDialog()
{
    delete ui;
}

void CurrencyNamesDialog::on_pbAdd_clicked()
{
    QModelIndex index;
    model->insertRow(model->rowCount(QModelIndex()), QModelIndex());
    index = model->index(model->rowCount(QModelIndex())-1, 1, QModelIndex());
    ui->lbTotal->setText(QString(tr("Total: %1").arg(model->rowCount(QModelIndex()))));

    ui->lvCurrencyNames->setFocus();
    ui->lvCurrencyNames->setCurrentIndex(index);
    ui->lvCurrencyNames->edit(index);
}


void CurrencyNamesDialog::on_pbRemove_clicked()
{
    QModelIndex index;
    index = ui->lvCurrencyNames->currentIndex();
    if(index.isValid())
    {
        model->removeRow(index.row(), QModelIndex());
        ui->lbTotal->setText(QString(tr("Total: %1").arg(model->rowCount(QModelIndex()))));
    }else
    {
        qDebug() << "invalid index";//
    }
}
