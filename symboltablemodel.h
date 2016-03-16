#ifndef SYMBOLTABLEMODEL_H
#define SYMBOLTABLEMODEL_H

#include "symboltableitem.h"
#include <QAbstractTableModel>

#include <unordered_map>


class SymbolTableModel final : public QAbstractTableModel
{
        Q_OBJECT

        struct Columns final : std::unordered_map<int, QString>
        {
            Columns ();

            enum eColumns
            {
                Unicode = 0,
                Char = 1,
                ScalableChar = 2
            };
        };

    public:
        SymbolTableModel(QObject *p);
        virtual ~SymbolTableModel ();

    public:
        void clear();
        void setRowCount(int rows);

        void setItem(int row, SymbolTableItem *item);
        QPixmap charPixmap(int row) const;

        QList<SymbolTableItem *> items() const
        {
            return m_items;
        }

    private:
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
        QList<SymbolTableItem *> m_items;
};

#endif // SYMBOLTABLEMODEL_H
