#include "tableview.h"


TableView::TableView(QWidget *parent) : QTableView(parent)
{
    setModel(new TableModel(this));
}

void TableView::setItem(int row, TableItem *item)
{
    model()->setItem(row, item);
}

void TableView::setRowCount(int rows)
{
    model()->setRowCount(rows);
}

QList<TableItem *> TableView::items() const
{
    return model()->items();
}
