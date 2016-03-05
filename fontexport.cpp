#include "fontexport.h"


IFontExport::IFontExport(const QList<TableItem *> &items)
{
    this->items = &items;
    positionInBitmap = 0;
    sizeBitmap = 0;
}

QString IFontExport::process()
{
    QString strBitmap, strSymbol;
    QTextStream streamBitmap(&strBitmap), streamSymbol(&strSymbol);

    quint8 height = 0;
    quint16 fstSymbol = 0;
    bool latch = false;
    foreach (TableItem * item, *items)
    {
        QPixmap pixmap = item->charPixmap().value<QPixmap>();
        QImage image = pixmap.toImage().convertToFormat(format, Qt::AvoidDither);

        if (latch)
            streamBitmap << ',' << endl << endl;
        else
        {
            height = image.height();
            fstSymbol = item->numericUnicode().toUInt();
        }
        CHAR_INFO smb = prepareBitmaps_CharInfo(image, streamBitmap);
        if (latch)
            streamSymbol << endl << ',';

        prepareCharInfo(item->numericUnicode().toChar(), smb, streamSymbol);
        latch = true;
    }

    QString oneBitColor = "struct ONE_BIT_COLOR\n"
                          "{\n"
                          "    const __uint8_t   height        = %1;\n"
                          "    const __uint16_t firstSymbol = %2;\n"
                          "    const struct CHAR_INFO\n"
                          "    {\n"
                          "        const __uint8_t   fstRow;\n"
                          "        const __uint8_t   lstRow;\n"
                          "        const __uint8_t   width;\n"
                          "        const __uint16_t position;\n"
                          "    } descriptors[%3] = {\n"
                          " %4\n"
                          "       };\n"
                          "    const __uint8_t bitmaps[%5] = {\n"
                          "%6\n"
                          "    };\n"
                          "};";
    return oneBitColor.arg(height).arg(fstSymbol).arg(items->size()).arg(strSymbol)
           .arg(sizeBitmap).arg(strBitmap);
}

// ###########################################################

QBitArray BitColor::scanLine(const quint8 index, const QImage &image) const
{
    const quint8 width = image.width();
    QBitArray bits(width);
    for (int n = 0; n != width; n++)
    {
        bool px = static_cast<bool>(static_cast<unsigned char>(image.pixel(n, index)));
        bits.setBit(n, px);
    }
    return bits;
}

quint8 BitColor::firstRow(const QImage &image, QBitArray &bits, bool &empty)
{
    const QBitArray EMPTY_LINE(image.width(), true);

    const quint8 height = image.height() - 1;
    quint8 fstRow = 0;
    do
    {
        bits = scanLine(fstRow, image);
        if (bits != EMPTY_LINE)
        {
            empty = false;
            return fstRow;
        }
        fstRow++;
    }
    while ( fstRow != height );

    empty = true;
    return fstRow;
}

quint8 BitColor::lastRow(const QImage &image, QBitArray &bits, bool &empty)
{
    const QBitArray EMPTY_LINE(image.width(), true);

    quint8 lstRow = image.height() - 1;
    do
    {
        bits = scanLine(lstRow, image);
        if (bits != EMPTY_LINE)
        {
            empty = false;
            return lstRow;
        }
        lstRow--;
    }
    while (lstRow != 0);

    empty = true;
    return lstRow;
}

QString BitColor::toString(const QBitArray &bit, QTextStream &stream)
{
    if (bit.isNull())
        return nullptr;

    QByteArray x;
    stream << "0b";
    for (int i = 0; i < bit.size(); i++)
    {
        if (i != 0 && i % 8 == 0)
            stream << ", 0b";
        stream << bit.testBit(i);
        x += bit.testBit(i) ? "░" : "█";
    }

    return QString(x);
}

IFontExport::CHAR_INFO BitColor::prepareBitmaps_CharInfo(const QImage &image,
                                                         QTextStream &bitmaps)
{
    bool isEmpty;

    quint16 size = 0;

    QBitArray fbits;
    quint8 fstRow = firstRow(image, fbits, isEmpty);
    if (!isEmpty)
    {
        bitmaps << "    /* " << toString(fbits, bitmaps) << "*/";
        size += sizeInByte(fbits);
    }
    bool latch = isEmpty;

    QBitArray lbits;
    quint8 lstRow = lastRow(image, lbits, isEmpty);
    for (quint8 i = fstRow + 1; i < lstRow; i++)
    {
        QBitArray bit = scanLine(i, image);
        if (!latch)
            bitmaps << ',' << endl;
        else
            latch = false;
        bitmaps << "    /* " << toString(bit, bitmaps) << "*/";
        size += sizeInByte(bit);
    }

    if ( !isEmpty )
    {
        if (!latch)
            bitmaps << ',' << endl;
        bitmaps << "    /* " << toString(lbits, bitmaps) << "*/";
        size += sizeInByte(lbits);
    }

    sizeBitmap += size;

    CHAR_INFO charInfo = { fstRow, lstRow, static_cast<const quint8>(image.width()),  positionInBitmap };
    positionInBitmap += size;

    return charInfo;
}
