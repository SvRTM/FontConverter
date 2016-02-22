#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QVariant>
#include <QPixmap>

class TableItem
{
    public:
    TableItem (short num, QPixmap data);

    public:
    void setNumber (QVariant num);
    QVariant number ();

    void setSymbolImage (QVariant image);
    QVariant symbolImage ();
    QVariant scaleSymbolImage ();

    private:
    short num;
    QPixmap data;
};

#endif // TABLEITEM_H
