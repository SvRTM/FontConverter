#include "tablemodel.h"
#include "tableitem.h"

TableModel::TableModel (QObject* p) : QAbstractTableModel (p)
{
    headers << "code"
            << "char"
            << "view";
}

QPixmap TableModel::getData (int row)
{
    if (row < 0 || row >= items.size ())
        return QPixmap ();

    TableItem *item = items[row];
    if (item==nullptr)
        return QPixmap ();

    return item->symbolImage ().value<QPixmap> ();
}

int TableModel::rowCount (const QModelIndex& parent) const
{
    return items.size ();
}

int TableModel::columnCount (const QModelIndex& parent) const
{
    return headers.size ();
}
/*
QVariant TableModel::data (const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        return Columns[index.column ()][index.row ()];
    }
    return QVariant::Invalid;
}*/

void TableModel::setItem (int row, TableItem* item)
{
    if (row < 0/* || row >= items.size ()*/)
        return;


    TableItem* oldItem = items[row];
    if (oldItem == item)
        return;
    if (oldItem != nullptr)
        delete oldItem;

    items[row] = item;

    QModelIndex idx = QAbstractTableModel::index (row, 0);
    QModelIndex idx2 = QAbstractTableModel::index (row, 2);
    emit dataChanged (idx, idx2);
}


QVariant TableModel::data (const QModelIndex& index, int role) const
{
    if (!index.isValid ())
        return QVariant ();

    if (role == Qt::DisplayRole || role == Qt::EditRole || role == Qt::DecorationRole)
    {
        TableItem* item = items[index.row ()];
        if (item != nullptr)
        {
            switch (index.column ())
            {
            case 0:
                return item->number ();
            case 1:
                return item->symbolImage ();
            case 2:
                return item->scaleSymbolImage ();
            }
        }
    }

    return QVariant ();
}

QVariant TableModel::headerData (int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal)
    {
        if (role == Qt::DisplayRole)
            return headers.at (section);
    }
    else if (role == Qt::DisplayRole)
        return section + 1;

    return QVariant ();
}
/*
bool TableModel::setData (const QModelIndex& index, const QVariant& value, int role)
{
    if (index.isValid () && role == Qt::EditRole)
    {
        TableItem* item = items[index.row ()];
        if (item != nullptr)
            item->setElement (index.column (), value);
        emit dataChanged (index, index);
        return true;
    }

    return false;
}*/
Qt::ItemFlags TableModel::flags (const QModelIndex& index) const
{
    return QAbstractTableModel::flags (index) | Qt::ItemIsEditable;
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
/*
bool TableModel::insertColumns (int position, int columns, const QModelIndex& parent)
{
    int rows = rowCount();
    beginInsertColumns (parent, position, position + columns - 1);

    /*for (int row = 0; row < rows; ++row) {
        for (int column = position; column < columns; ++column) {
            rowList[row].insert(position, "");
        }
    }/

    endInsertColumns ();
    return true;
}
    */
/*
bool TableModel::removeColumns (int position, int columns, const QModelIndex& parent)
{
    int rows = rowCount();
    beginRemoveColumns (parent, position, position + columns - 1);

    /*for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {
            rowList[row].removeAt(position);
        }
    }*

    endRemoveColumns ();
    return true;
}*/
/*
bool TableModel::setData(const QModelIndex& index,
             const QVariant&    value,
             int                nRole
            )
{
    if (index.isValid() && nRole == Qt::EditRole) {
        m_hash[index] = value;
        emit dataChanged(index, index);
        return true;
    }
    return false;
}
*/
/*
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

  if(role == Qt::DisplayRole)
    {
    std::stringstream ss;
    if(orientation == Qt::Horizontal)
      {
      ss << "H_" << section;
      return QString(ss.str().c_str());
      }
    else if(orientation == Qt::Vertical)
      {
      ss << "V_" << section;
      return QString(ss.str().c_str());
      }

    }

  return QVariant::Invalid;
}
*/
/*
bool MyModel::insertColumn(int column, const QString &title) {
    if (column >= 0 && column <= mColumns.size()) {
        beginInsertColumns(QModelIndex(), column, column);
        mColumns.insert(column, title);
        int size = mModelData.size();
        for (int i = 0; i < size; ++i) {
            mModelData[i].insert(column, qrand());
        }
        endInsertColumns();
        return true;
    }
    return false;
}

bool MyModel::removeColumn(int column) {
    if (column >= 0 && column < mColumns.size()) {
        beginRemoveColumns(QModelIndex(), column, column);
        mColumns.removeAt(column);
        int size = mModelData.size();
        for (int i = 0; i < size; ++i) {
            mModelData[i].removeAt(column);
        }
        endRemoveColumns();
        return true;
    }
    return false;
}


/*

TableView::TableView() : QTableView(NULL)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::DoubleClicked |  QAbstractItemView::EditKeyPressed);
    setTabKeyNavigation(false);
    setSelectionMode(QAbstractItemView::SingleSelection);
    verticalHeader()->setResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(18);
    setAlternatingRowColors(true);
    firstColumn = 1;
    isEditable = true;
}

void TableView::keyPressEvent(QKeyEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {

        if (event->key() == Qt::Key_Insert)
        {
            addRow();
        }

        if (event->key() == Qt::Key_Return)
        {
                if (state()!= QAbstractItemView::EditingState && isEditable)
            {
                        editingCell = currentIndex();
                        edit(editingCell);
                    }
        }

        if (event->key() == Qt::Key_Delete && event->modifiers() | Qt::Key_Control)
        {
            deleteRow();
            return;
        }
   }
        QTableView::keyPressEvent(event);
}


void TableView::setFirstColumn(int column)
{
    firstColumn = column;
}

void TableView::addRow()
{
        QSqlTableModel *mdl = dynamic_cast<QSqlTableModel*>(model());
        qDebug() << "inserting a row";

        int row =  currentIndex().row() != -1? currentIndex().row():0;
        if (mdl->insertRow(row))
        {
            addingrow = true;
            QModelIndex index = mdl->index(row, firstColumn);
            if (index.isValid())
            {
                setCurrentIndex(index);
                editingCell = index;
                edit(index);
            }
        }
}

void TableView::deleteRow()
{
    if (state()== QTableView::EditingState || !isEditable)
        return;
        QSqlTableModel *mdl = dynamic_cast<QSqlTableModel*>(model());
        qDebug() << "removing a row";
        editingCell = currentIndex();
        if (editingCell.isValid())
        {
            mdl->removeRows(editingCell.row(), 1);
            selectRow(editingCell.row());
        }
}

void TableView::setEditable(bool editable)
{
    isEditable = editable;
}

bool TableView::editable()
{
    return isEditable;
}

void TableView::closeEditor(QWidget *editor, QAbstractItemDelegate::EndEditHint hint)
{
    //Хорошо работает для простых QLineEdit, но сегфолится на QDateEdit.
    //Непонятно, что ему надо. Похоже, дело во внутренней event-кухне при получении фокуса
    setCurrentIndex(editingCell);
    setFocus(Qt::OtherFocusReason);
    QTableView::closeEditor(editor, hint);
}

}
*/
