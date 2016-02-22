#include "tableitem.h"
#include <QPixmap>
#include <QPainter>

TableItem::TableItem (short number, QPixmap data)
{
    this->num = number;
    this->data = data;
}

void TableItem::setNumber (QVariant number)
{
    this->num = number.toInt ();
}
QVariant TableItem::number ()
{
    return num;
}

void TableItem::setSymbolImage (QVariant image)
{
    data = image.value<QPixmap> ();
}
QVariant TableItem::symbolImage ()
{
    return data ;
}
QVariant TableItem::scaleSymbolImage ()
{
    int width = data.width ();
    int height = data.height ();
    QPixmap pixmap = data.scaled (width * 10, height * 10, Qt::KeepAspectRatio);
    QPainter painter (&pixmap);
    painter.setPen (Qt::gray);
    for (int n = 0; n <= width * 10; n += 10)
        painter.drawLine (n, 0, n, height * 10);
    for (int n = 0; n <= height * 10; n += 10)
        painter.drawLine (0, n, width * 10, n);
    return pixmap;
}
