#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "tableitem.h"
#include <QAbstractTableModel>
#include <unordered_map>


class TableModel final : public QAbstractTableModel
{
        Q_OBJECT

        struct Columns final : std::unordered_map<int, QString>
        {
            Columns ();
            virtual ~Columns() {}

            enum eColumns
            {
                Unicode = 0,
                Char = 1,
                ScalableChar = 2
            };
        };

    public:
        TableModel(QObject *p);
        virtual ~TableModel ();

    public:
        void setRowCount(int rows);

        void setItem(int row, TableItem *item);
        QPixmap charPixmap(int row) const;

        QList<TableItem *> items() const
        {
            return m_items;
        }

    private:
        void clear();

        virtual QVariant data(const QModelIndex &index, int role) const override;
        virtual QVariant headerData(int section, Qt::Orientation orientation,
                                     int role) const override;
        virtual bool insertRows(int position, int rows,
                                 const QModelIndex &parent = QModelIndex()) override;
        virtual bool removeRows(int position, int rows,
                                 const QModelIndex &parent = QModelIndex()) override;

        virtual int columnCount (const QModelIndex &parent = QModelIndex()) const override
        {
            Q_UNUSED (parent)
            return m_headers.size();
        }

        virtual int rowCount (const QModelIndex &parent = QModelIndex()) const override
        {
            Q_UNUSED (parent)
            return m_items.size();
        }

    private:
        const Columns m_headers;
        QList<TableItem *> m_items;
};

#endif // TABLEMODEL_H
