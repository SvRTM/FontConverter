/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

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
