#include "tableview.h"
#include "tablemodel.h"

TableView::TableView (QWidget* parent) : QTableView (parent)
{
}

void TableView::setItem (int row, TableItem* item)
{
    //    if (item == nullptr)
    //        delete takeItem(row, column);

    (qobject_cast<TableModel*> (model ()))->setItem (row, item);
   // setRowHeight (row, item->symbolImage ().value<QPixmap> ().height () * 10 + 10);
}
/*
TableItem *TableView::takeItem(int row, int column)
{
    Q_D(QTableWidget);
    QTableWidgetItem *item = d->tableModel()->takeItem(row, column);
    if (item)
        item->view = 0;
    return item;
}*/

void TableView::setRowCount (int rows)
{
    model ()->insertRows (model ()->rowCount (), rows);
}

void TableView::setColumnCount (int n)
{
}

void TableView::setHorizontalHeaderLabels (QStringList headers)
{
}
