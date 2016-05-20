#ifndef CHARACTERINFOTABLEVIEW_H
#define CHARACTERINFOTABLEVIEW_H

#include "Widgets/characterInfoTableItem.h"
#include "Widgets/TableModel/characterInfoTableModel.h"

#include <QTableView>

class CharacterInfoTableView final : public QTableView
{
        Q_OBJECT

    public:
        explicit CharacterInfoTableView(QWidget *pParent);
        virtual ~CharacterInfoTableView() {}

    public:
        void setItem(int row, CharacterInfoTableItem *pItem);
        void setRowCount(int n);
        QList<CharacterInfoTableItem *> items() const;

        inline CharacterInfoTableModel *model() const
        {
            return (qobject_cast<CharacterInfoTableModel *>(QTableView::model()));
        }
};

#endif // CHARACTERINFOTABLEVIEW_H
