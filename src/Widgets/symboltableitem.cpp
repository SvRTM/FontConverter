#include "symboltableitem.h"
#include <QPainter>

SymbolTableItem::SymbolTableItem(quint16 numericUnicode, QPixmap charPixmap)
{
    this->m_numericUnicode = numericUnicode;
    this->m_charPixmap = charPixmap;
}

QVariant SymbolTableItem::charPixmap()
{
    return m_charPixmap;
}

QVariant SymbolTableItem::scaleCharPixmap()
{
    int width = m_charPixmap.width();
    int height = m_charPixmap.height();

    QPixmap pixmap = m_charPixmap.scaled(width * 5, height * 5, Qt::KeepAspectRatio);
    QPainter painter(&pixmap);

    painter.setPen(Qt::gray);
    for (int n = 5; n < width * 5; n += 5)
        painter.drawLine (n, 0, n, height * 5);
    for (int n = 5; n < height * 5; n += 5)
        painter.drawLine (0, n, width * 5, n);

    return pixmap;
}
