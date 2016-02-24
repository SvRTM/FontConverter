#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QVariant>
#include <QPixmap>

class TableItem final
{
    public:
    explicit TableItem (ushort numericUnicode, QPixmap charPixmap);

    public:
    void setNumericUnicode (QVariant numericUnicode);
    QVariant numericUnicode ();

    void setCharPixmap (QVariant charPixmap);
    QVariant charPixmap ();
    QVariant scaleCharPixmap ();

    private:
    short m_numericUnicode;
    QPixmap m_charPixmap;
};

#endif // TABLEITEM_H
