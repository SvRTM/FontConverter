#include "characterInfoTableView.h"

CharacterInfoTableView::CharacterInfoTableView(QWidget *pParent)
    : QTableView(pParent)
{
    setModel(new CharacterInfoTableModel(this));
}

void CharacterInfoTableView::setItem(int row, CharacterInfoTableItem *pItem)
{
    model()->setItem(row, pItem);
}

void CharacterInfoTableView::setRowCount(int rows)
{
    model()->setRowCount(rows);
}

QList<CharacterInfoTableItem *> CharacterInfoTableView::items() const
{
    return model()->items();
}
