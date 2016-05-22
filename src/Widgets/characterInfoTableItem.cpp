/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

#include "characterInfoTableItem.h"
#include <QPainter>

CharacterInfoTableItem::CharacterInfoTableItem(quint16 numericUnicode, QPixmap charPixmap)
{
    this->m_numericUnicode = numericUnicode;
    this->m_charPixmap = charPixmap;
}

QVariant CharacterInfoTableItem::charPixmap()
{
    return m_charPixmap;
}

QVariant CharacterInfoTableItem::scaleCharPixmap()
{
    const int width = m_charPixmap.width();
    if (width == 0)
        return QPixmap();

    const int height = m_charPixmap.height();
    QPixmap pixmap = m_charPixmap.scaled(width * 5, height * 5, Qt::KeepAspectRatio);
    QPainter painter(&pixmap);

    painter.setPen(Qt::gray);
    QVarLengthArray<QLine> lines;
    lines.reserve((width + height)  - 2);
    for (int n = zoom; n < width * zoom; n += zoom)
        lines.append(QLine(n, 0, n, height * zoom));
    for (int n = zoom; n < height * zoom; n += zoom)
        lines.append(QLine(0, n, width * zoom, n));
    painter.drawLines(lines.data(), lines.size());

    return pixmap;
}
