#ifndef SYMBOLTABLEITEM_H
#define SYMBOLTABLEITEM_H

#include <QVariant>
#include <QPixmap>


class SymbolTableItem final
{
    public:
        explicit SymbolTableItem(quint16 numericUnicode, QPixmap charPixmap);

    public:
        QVariant charPixmap();
        QVariant scaleCharPixmap();

        quint16 numericUnicode() const
        {
            return m_numericUnicode;
        }

    private:
        quint16 m_numericUnicode;
        QPixmap m_charPixmap;
};

#endif // SYMBOLTABLEITEM_H
