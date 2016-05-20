#ifndef CHARACTERINFOTABLEMODEL_H
#define CHARACTERINFOTABLEMODEL_H

#include "Widgets/characterInfoTableItem.h"
#include <QAbstractTableModel>

#include <unordered_map>

class CharacterInfoTableModel final : public QAbstractTableModel
{
        Q_OBJECT

        struct Columns final : std::unordered_map<int, QString>
        {
            Columns();

            enum eColumns
            {
                Unicode = 0,
                Char = 1,
                ScalableChar = 2
            };
        };

    public:
        CharacterInfoTableModel(QObject *pParent) : QAbstractTableModel(pParent) {}
        virtual ~CharacterInfoTableModel()
        {
            clear();
        }

    public:
        void clear();
        void setRowCount(int rows);

        void setItem(int row, CharacterInfoTableItem *pItem);
        QPixmap charPixmap(int row) const;

        QList<CharacterInfoTableItem *> items() const
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

        virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override
        {
            Q_UNUSED(parent)
            return m_headers.size();
        }

        virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override
        {
            Q_UNUSED(parent)
            return m_items.size();
        }

    private:
        const Columns m_headers;
        QList<CharacterInfoTableItem *> m_items;
};

#endif // CHARACTERINFOTABLEMODEL_H
