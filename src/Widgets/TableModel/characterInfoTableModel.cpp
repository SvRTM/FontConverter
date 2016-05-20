#include "characterInfoTableModel.h"

CharacterInfoTableModel::Columns::Columns ()
{
    operator[](Unicode) = "Numeric unicode";
    operator[](Char) = "Char";
    operator[](ScalableChar) = "Scalable char";
}

void CharacterInfoTableModel::clear()
{
    removeRows(0, rowCount());
}

void CharacterInfoTableModel::setRowCount(int rows)
{
    int rc = rowCount();
    if (rows < 0 || rc == rows)
        return;
    if (rc < rows)
        insertRows(rc, rows - rc);
    else
        removeRows(rows, rc - rows);
}

QPixmap CharacterInfoTableModel::charPixmap(int row) const
{
    if (row < 0 || row >= m_items.size())
        return QPixmap();

    CharacterInfoTableItem *pItem = m_items[row];
    if (pItem == nullptr)
        return QPixmap();

    return pItem->charPixmap().value<QPixmap>();
}

void CharacterInfoTableModel::setItem(int row, CharacterInfoTableItem *pItem)
{
    if (row < 0 || row >= m_items.size())
        return;

    CharacterInfoTableItem *pOldItem = m_items[row];
    if (pOldItem == pItem)
        return;
    if (pOldItem != nullptr)
        delete pOldItem;

    m_items[row] = pItem;

    emit dataChanged(index(row, Columns::Unicode), index(row, Columns::ScalableChar));
}

QVariant CharacterInfoTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::DecorationRole)
    {
        CharacterInfoTableItem *pItem = m_items[index.row()];
        if (pItem != nullptr)
            switch (index.column())
            {
                case Columns::Unicode:
                    return pItem->numericUnicode();
                case Columns::Char:
                    return pItem->charPixmap();
                case Columns::ScalableChar:
                    return pItem->scaleCharPixmap();
            }
    }

    return QVariant();
}

QVariant CharacterInfoTableModel::headerData(int section, Qt::Orientation orientation,
        int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
            return m_headers.at(section);
    }
    else if (role == Qt::DisplayRole)
        return section + 1;

    return QVariant();
}

bool CharacterInfoTableModel::insertRows(int position, int rows,
        const QModelIndex &parent)
{
    if (rows < 1 || position < 0)
        return false;

    beginInsertRows(parent, position, position + rows - 1);

    for (int i = 0; i < rows; ++i)
        m_items.insert(position + i, nullptr);

    endInsertRows();

    return true;
}

bool CharacterInfoTableModel::removeRows(int position, int rows,
        const QModelIndex &parent)
{
    beginRemoveRows(parent, position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
        CharacterInfoTableItem *pItem = m_items[position];
        if (pItem != nullptr)
            delete pItem;
        m_items.removeAt(position);
    }

    endRemoveRows();

    return true;
}
