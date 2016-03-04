#include "tableitem.h"
#include <QPainter>


TableItem::TableItem(quint16 numericUnicode, QPixmap charPixmap)
{
    this->m_numericUnicode = numericUnicode;
    this->m_charPixmap = charPixmap;
}

void TableItem::setNumericUnicode(QVariant numericUnicode)
{
    this->m_numericUnicode = numericUnicode.toInt ();
}

QVariant TableItem::numericUnicode()
{
    return m_numericUnicode;
}

void TableItem::setCharPixmap(QVariant charPixmap)
{
    m_charPixmap = charPixmap.value<QPixmap>();
}

QVariant TableItem::charPixmap()
{
    return m_charPixmap;
}

QVariant TableItem::scaleCharPixmap()
{
    int width = m_charPixmap.width();
    int height = m_charPixmap.height();

    QPixmap pixmap = m_charPixmap.scaled(width * 5, height * 5,
                                         Qt::KeepAspectRatio);
    QPainter painter(&pixmap);

    painter.setPen(Qt::gray);
    for (int n = 5; n < width * 5; n += 5)
        painter.drawLine (n, 0, n, height * 5);
    for (int n = 5; n < height * 5; n += 5)
        painter.drawLine (0, n, width * 5, n);

    return pixmap;
}
