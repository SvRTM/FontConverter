#ifndef SYMBOLTABLEVIEW_H
#define SYMBOLTABLEVIEW_H

#include "../symboltableitem.h"
#include "../TableModel/symboltablemodel.h"

#include <QTableView>


class SymbolTableView final : public QTableView
{
        Q_OBJECT

    public:
        explicit SymbolTableView(QWidget *parent);
        virtual ~SymbolTableView(){}

    public:
        void setItem(int row, SymbolTableItem *item);
        void setRowCount(int n);
        QList<SymbolTableItem *> items() const;

        inline SymbolTableModel *model() const
        {
            return (qobject_cast<SymbolTableModel *>(QTableView::model()));
        }
};

#endif // SYMBOLTABLEVIEW_H
