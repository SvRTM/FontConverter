#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QVariant>
#include <QPixmap>


class TableItem final
{
    public:
        explicit TableItem(quint16 numericUnicode, QPixmap charPixmap);

    public:
        void setNumericUnicode(QVariant numericUnicode);
        QVariant numericUnicode();

        void setCharPixmap(QVariant charPixmap);
        QVariant charPixmap();
        QVariant scaleCharPixmap();

    private:
        quint16 m_numericUnicode;
        QPixmap m_charPixmap;
};

#endif // TABLEITEM_H
