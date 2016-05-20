#ifndef CHARACTERINFOTABLEITEM_H
#define CHARACTERINFOTABLEITEM_H

#include <QVariant>
#include <QPixmap>

class CharacterInfoTableItem final
{
    public:
        explicit CharacterInfoTableItem(quint16 numericUnicode, QPixmap charPixmap);

    public:
        QVariant charPixmap();
        QVariant scaleCharPixmap();

        quint16 numericUnicode() const
        {
            return m_numericUnicode;
        }

    private:
        static constexpr quint8 zoom = 5;

        quint16 m_numericUnicode;
        QPixmap m_charPixmap;
};

#endif // CHARACTERINFOTABLEITEM_H
