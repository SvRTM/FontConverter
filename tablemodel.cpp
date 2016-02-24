#include "tablemodel.h"

TableModel::Columns::Columns ()
{
    operator[](Unicode) = "numeric unicode";
    operator[](Char) = "char";
    operator[](ScalableChar) = "scalable char";
}


TableModel::TableModel (QObject* p) : QAbstractTableModel (p)
{
}
TableModel::~TableModel ()
{
    clear ();
}

void TableModel::clear ()
{
    for (TableItem* item : items)
        delete item;
    items.clear ();
}

void TableModel::setRowCount (int rows)
{
    int rc = rowCount ();
    if (rows < 0 || rc == rows)
        return;
    if (rc < rows)
        insertRows (rc, rows - rc);
    else
        removeRows (rows, rc - rows);
}

QPixmap TableModel::charPixmap (int row)
{
    if (row < 0 || row >= items.size ())
        return QPixmap ();

    TableItem* item = items[row];
    if (item == nullptr)
        return QPixmap ();

    return item->charPixmap ().value<QPixmap> ();
}

void TableModel::setItem (int row, TableItem* item)
{
    if (row < 0 || row >= items.size ())
        return;

    TableItem* oldItem = items[row];
    if (oldItem == item)
        return;
    if (oldItem != nullptr)
        delete oldItem;

    items[row] = item;

    emit dataChanged (index (row, Columns::Unicode), index (row, Columns::ScalableChar));
}

QVariant TableModel::data (const QModelIndex& index, int role) const
{
    if (!index.isValid ())
        return QVariant ();

    if (role == Qt::DisplayRole || role == Qt::DecorationRole)
    {
        TableItem* item = items[index.row ()];
        if (item != nullptr)
            switch (index.column ())
            {
            case Columns::Unicode:
                return item->numericUnicode ();
            case Columns::Char:
                return item->charPixmap ();
            case Columns::ScalableChar:
                return item->scaleCharPixmap ();
            }
    }

    return QVariant ();
}

QVariant TableModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
            return m_headers.at (section);
    }
    else if (role == Qt::DisplayRole)
        return section + 1;

    return QVariant ();
}

bool TableModel::insertRows (int position, int rows, const QModelIndex& parent)
{
    if (rows < 1 || position < 0)
        return false;

    beginInsertRows (parent, position, position + rows - 1);

    for (int i = 0; i < rows; ++i)
        items.insert (position + i, nullptr);

    endInsertRows ();

    return true;
}

bool TableModel::removeRows (int position, int rows, const QModelIndex& parent)
{
    beginRemoveRows (parent, position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
    {
        TableItem* item = items[position];
        if (item != nullptr)
            delete item;
        items.removeAt (position);
    }

    endRemoveRows ();

    return true;
}
