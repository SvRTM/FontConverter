#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "tableitem.h"

#include <vector>
#include <sstream>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
    TableModel (QObject* p);

    void setItem (int row, TableItem* item);
    QPixmap getData (int row);


    int rowCount (const QModelIndex& parent = QModelIndex ()) const;
    int columnCount (const QModelIndex& parent = QModelIndex ()) const;
    QVariant data (const QModelIndex& index, int role) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role) const;
    //bool setData (const QModelIndex& index, const QVariant& value, int role);
    Qt::ItemFlags flags (const QModelIndex& index) const;
    bool insertRow (int row, const QModelIndex& parent);
    bool insertRows (int position, int rows, const QModelIndex& parent);
    bool removeRows (int position, int rows, const QModelIndex& parent);
    //bool insertColumns (int position, int columns, const QModelIndex& parent);
    //bool removeColumns (int position, int columns, const QModelIndex& parent);


    //inline long tableIndex(int row, int column) const
    //        { return (row * horizontalHeaderItems.count()) + column; }

    protected:
    QStringList headers;
    QList<TableItem*> items;
};

#endif // TABLEMODEL_H
