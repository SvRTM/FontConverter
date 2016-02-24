#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "tableitem.h"
#include "tablemodel.h"

#include <QTableView>


class TableView final : public QTableView
{
    Q_OBJECT

    public:
    explicit TableView (QWidget* parent = 0);
    virtual ~TableView ()
    {
    }

    public:
    void setItem (int row, TableItem* item);
    void setRowCount (int n);

    inline TableModel* model ()
    {
        return (qobject_cast<TableModel*> (QTableView::model ()));
    }
};

#endif // TABLEVIEW_H
