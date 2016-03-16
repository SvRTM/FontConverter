#include "symboltablemodel.h"


SymbolTableModel::Columns::Columns ()
{
    operator[](Unicode) = "Numeric unicode";
    operator[](Char) = "Char";
    operator[](ScalableChar) = "Scalable char";
}


SymbolTableModel::SymbolTableModel(QObject *p) : QAbstractTableModel(p)
{}
SymbolTableModel::~SymbolTableModel()
{
    clear();
}

void SymbolTableModel::clear()
{
    removeRows(0,rowCount());
}

void SymbolTableModel::setRowCount(int rows)
{
    int rc = rowCount();
    if (rows < 0 || rc == rows)
        return;
    if (rc < rows)
        insertRows(rc, rows - rc);
    else
        removeRows(rows, rc - rows);
}

QPixmap SymbolTableModel::charPixmap(int row) const
{
    if (row < 0 || row >= m_items.size())
        return QPixmap();

    SymbolTableItem *item = m_items[row];
    if (item == nullptr)
        return QPixmap();

    return item->charPixmap().value<QPixmap>();
}

void SymbolTableModel::setItem (int row, SymbolTableItem *item)
{
    if (row < 0 || row >= m_items.size())
        return;

    SymbolTableItem *oldItem = m_items[row];
    if (oldItem == item)
        return;
    if (oldItem != nullptr)
        delete oldItem;

    m_items[row] = item;

    emit dataChanged(index(row, Columns::Unicode), index (row, Columns::ScalableChar));
}

QVariant SymbolTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::DecorationRole)
    {
        SymbolTableItem *item = m_items[index.row()];
        if (item != nullptr)
            switch (index.column ())
            {
                case Columns::Unicode:
                    return item->numericUnicode();
                case Columns::Char:
                    return item->charPixmap();
                case Columns::ScalableChar:
                    return item->scaleCharPixmap();
            }
    }

    return QVariant();
}

QVariant SymbolTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool SymbolTableModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    if (rows < 1 || position < 0)
        return false;

    beginInsertRows(parent, position, position + rows - 1);

    for (int i = 0; i < rows; ++i)
        m_items.insert(position + i, nullptr);

    endInsertRows();

    return true;
}

bool SymbolTableModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    beginRemoveRows(parent, position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
        SymbolTableItem *item = m_items[position];
        if (item != nullptr)
            delete item;
        m_items.removeAt(position);
    }

    endRemoveRows();

    return true;
}
