#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QTableView>
#include "tableitem.h"

class TableView : public QTableView
{
    Q_OBJECT

    public:
    explicit TableView (QWidget* parent = 0);

    void setItem (int row, TableItem* item);
    void setRowCount (int n);
    void setColumnCount (int n);
    void setHorizontalHeaderLabels (QStringList headers);
};

#endif // TABLEVIEW_H
