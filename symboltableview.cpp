#include "symboltableview.h"


SymbolTableView::SymbolTableView(QWidget *parent) : QTableView(parent)
{
    setModel(new SymbolTableModel(this));
}

void SymbolTableView::setItem(int row, SymbolTableItem *item)
{
    model()->setItem(row, item);
}

void SymbolTableView::setRowCount(int rows)
{
    model()->setRowCount(rows);
}

QList<SymbolTableItem *> SymbolTableView::items() const
{
    return model()->items();
}
