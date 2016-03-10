#include "fontexport.h"
#include <algorithm>

IFontExport::IFontExport(QList<TableItem *> items)
{
    this->items = items;
    positionInBitmap = 0;
    sizeBitmap = 0;
}

QString IFontExport::process()
{
    QString strBitmap, strSymbol, strBlock;
    QTextStream streamBitmap(&strBitmap), streamSymbol(&strSymbol),
                streamBlock(&strBlock);

    std::sort(items.begin(), items.end(), [] (const TableItem * a,
                                              const TableItem * b) -> bool {return a->numUnicode() < b->numUnicode();});

    quint8 height;
    quint16 startChar, prevChar, nDescriptor = 0, sizeBlocks = 0;
    bool latch = false;
    for (int n = 0; n < items.size(); n++)
    {
        TableItem *item = items[n];
        QPixmap pixmap = item->charPixmap().value<QPixmap>();
        QImage image = pixmap.toImage().convertToFormat(format, Qt::AvoidDither);

        quint16 currChar = item->numUnicode();

        if (latch)
        {
            streamBitmap << ',' << endl << endl;
            if (prevChar != currChar - 1)
            {
                prepareBlock(startChar, prevChar, nDescriptor, streamBlock);

                startChar = currChar;
                nDescriptor = n;
                sizeBlocks++;
            }
        }
        else
        {
            height = image.height();
            startChar = currChar;

        }
        prevChar = currChar;

        CHAR_INFO smb = prepareBitmaps_CharInfo(image, streamBitmap);
        prepareCharInfo(currChar, smb, streamSymbol, latch);

        latch = true;
    }
    prepareBlock(startChar, prevChar, nDescriptor, streamBlock, false);

    QString oneBitColor = "extern const struct ONE_BIT_COLOR\n"
                          "{\n"
                          "    const uint8_t    height        = %1;\n"
                          "    const struct CHAR_INFO\n"
                          "    {\n"
                          "        const uint8_t   fstRow;\n"
                          "        const uint8_t   lstRow;\n"
                          "        const uint8_t   width;\n"
                          "        const uint16_t position;\n"
                          "    } descriptors[%2] = {\n"
                          "             %3\n"
                          "       };\n"
                          "\n"
                          "    const struct BLOCK\n"
                          "    {\n"
                          "        const uint16_t    startChar;\n"
                          "        const uint16_t    endChar;\n"
                          "        const CHAR_INFO *descriptors;\n"
                          "    } blocks[%4] = {\n"
                          "%5\n"
                          "       };\n"
                          "\n"
                          "    const uint8_t bitmaps[%6] = {\n"
                          "%7\n"
                          "    };\n"
                          "};";

    return oneBitColor.arg(height).arg(items.size()).arg(strSymbol)
           .arg(sizeBlocks + 1).arg(strBlock)
           .arg(sizeBitmap).arg(strBitmap);
}

// ###########################################################

QBitArray BitColor::scanLine(const quint8 index, const QImage &image) const
{
    const quint8 width = image.width();
    QBitArray bits(width);
    for (int n = 0; n != width; n++)
    {
        bool px = static_cast<bool>(static_cast<quint8>(image.pixel(n, index)));
        bits.setBit(n, px);
    }
    return bits;
}

quint8 BitColor::firstRow(const QImage &image, QBitArray &bits)
{
    const QBitArray EMPTY_LINE(image.width(), true);

    const quint8 height = image.height() - 1;
    quint8 fstRow = 0;
    do
    {
        bits = scanLine(fstRow, image);
        if (bits != EMPTY_LINE)
            break;
        fstRow++;
    }
    while ( fstRow != height );

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
    return image.height() - 1;
}

QString BitColor::toString(const QBitArray &bit, QTextStream &stream)
{
    if (bit.isNull())
        return nullptr;

    QByteArray x;
    stream << "        0b";
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
    quint16 size = 0;

    QBitArray fbits;
    quint8 fstRow = firstRow(image, fbits);
    bitmaps << "    /* " << toString(fbits, bitmaps) << "*/";
    size += sizeInByte(fbits);

    bool latch = false;
    bool isEmpty;
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

// ###########################################################

QByteArray GrayscaleColor::scanLine(const quint8 index,
                                    const QImage &image) const
{
    const quint8 width = image.width();
    QByteArray bytes;
    for (int n = 0; n != width; n++)
    {
        quint8 px = static_cast<quint8>(image.pixel(n, index));
        bytes.append(px);
    }
    return bytes;
}

quint8 GrayscaleColor::firstRow(const QImage &image, QByteArray &bytes)
{
    const QByteArray EMPTY_LINE(image.width(), 255);

    const quint8 height = image.height() - 1;
    quint8 fstRow = 0;
    do
    {
        bytes = scanLine(fstRow, image);
        if (bytes != EMPTY_LINE)
            break;
        fstRow++;
    }
    while ( fstRow != height );

    return fstRow;
}

quint8 GrayscaleColor::lastRow(const QImage &image, QByteArray &bytes,
                               bool &empty)
{
    const QByteArray EMPTY_LINE(image.width(), 255);

    quint8 lstRow = image.height() - 1;
    do
    {
        bytes = scanLine(lstRow, image);
        if (bytes != EMPTY_LINE)
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

void GrayscaleColor::toString(const QByteArray &byte, QTextStream &stream)
{
    if (byte.isNull())
        return;

    QString str;
    QTextStream stm(&str);
    for (int i = 0; i < byte.size(); i++)
    {
        quint8 b = static_cast<quint8>(byte[i]);

        if (i != 0)
            stm << ", " ;
        else
            stream << "/* ";
        stream << QChar( b < 51 ?  0x2588 :
                         b >= 51 && b < 102 ? 0x2593 :
                         b >= 102 && b < 153 ? 0x2592 :
                         b >= 153 && b < 204 ? 0x2591
                         : 0x2581 );
        stm << "0x" << uppercasedigits << hex << b;
    }
    stream << " */\t\t" << str;
}

IFontExport::CHAR_INFO GrayscaleColor::prepareBitmaps_CharInfo(
    const QImage &image,
    QTextStream &bitmaps)
{
    quint16 size = 0;

    QByteArray fbytes;
    quint8 fstRow = firstRow(image, fbytes);
    toString(fbytes, bitmaps) ;
    size += fbytes.size();

    bool latch = false;
    bool isEmpty;
    QByteArray lbytes;
    quint8 lstRow = lastRow(image, lbytes, isEmpty);
    for (quint8 i = fstRow + 1; i < lstRow; i++)
    {
        QByteArray byte = scanLine(i, image);
        if (!latch)
            bitmaps << ',' << endl;
        else
            latch = false;
        toString(byte, bitmaps);
        size += byte.size();
    }

    if ( !isEmpty )
    {
        if (!latch)
            bitmaps << ',' << endl;
        toString(lbytes, bitmaps);
        size += lbytes.size();
    }

    sizeBitmap += size;

    CHAR_INFO charInfo = { fstRow, lstRow, static_cast<const quint8>(image.width()),  positionInBitmap };
    positionInBitmap += size;

    return charInfo;
}
