#include "fontexport.h"
#include <algorithm>

IFontExport::IFontExport(const QString &_fontName,
                         QList<SymbolTableItem *> _items,
                         QImage::Format _format) : fontName(_fontName), items(_items), format(_format),
    positionInBitmap(0), sizeBitmap(0)
{
}

QString IFontExport::process()
{
    QString strBitmap, strSymbol, strBlock;
    QTextStream streamBitmap(&strBitmap), streamSymbol(&strSymbol), streamBlock(&strBlock);

    std::sort(items.begin(), items.end(), [] (const SymbolTableItem * const a,
                                              const SymbolTableItem * const b) -> bool
    {
        return a->numericUnicode() < b->numericUnicode();
    });

    quint8 height;
    quint16 startChar = 0, prevChar = 0, nDescriptor = 0, sizeBlocks = 0;
    bool latch = false;
    for (int n = 0; n < items.size(); ++n)
    {
        SymbolTableItem  *item = items[n];
        QPixmap pixmap = item->charPixmap().value<QPixmap>();
        QImage image = pixmap.toImage().convertToFormat(format, Qt::AvoidDither);

        quint16 currChar = item->numericUnicode();

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

    QString fontStruct = "struct %1\n"
                         "{\n"
                         "    const uint8_t    height        = %2;\n"
                         "    const struct CHAR_INFO\n"
                         "    {\n"
                         "        const uint8_t   fstRow;\n"
                         "        const uint8_t   sizeRow;\n"
                         "        const uint8_t   width;\n"
                         "        const %3 position;\n"
                         "    } descriptors[%4] = {\n"
                         "             %5\n"
                         "       };\n"
                         "\n"
                         "    const struct BLOCK\n"
                         "    {\n"
                         "        const uint16_t    startChar;\n"
                         "        const uint16_t    endChar;\n"
                         "        const CHAR_INFO *descriptors;\n"
                         "    } blocks[%6] = {\n"
                         "%7\n"
                         "       };\n"
                         "\n"
                         "    const uint8_t bitmaps[%8] = {\n"
                         "%9\n"
                         "    };\n"
                         "};";

    return fontStruct.arg(fontName)
           .arg(height).arg(size_t_PosBimap(sizeBitmap)).arg(items.size()).arg(strSymbol)
           .arg(sizeBlocks + 1).arg(strBlock)
           .arg(sizeBitmap).arg(strBitmap);
}

// ###########################################################

QBitArray BitColor::scanLine(const quint8 index, const QImage &image) const
{
    const quint8 width = image.width();
    QBitArray bits(width);
    for (int n = 0; n != width; ++n)
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
    stream << "        " << (CxxStandart::Cxx14 == cxx ? "0b" : "0x");

    if (CxxStandart::Cxx14 == cxx)
        for (int i = 0; i < bit.size(); i++)
        {
            if (i != 0 && i % 8 == 0)
                stream << ", 0b";
            bool b = bit.testBit(i);
            stream << b;

            x += b ? "░" : "█";
        }
    else
    {
        quint8 v = 0;
        for (int i = 0; i < bit.size(); i++)
        {
            if (i != 0 && i % 8 == 0)
            {
                stream << uppercasedigits << hex << v << "U, 0x";
                v = 0;
            }
            bool b = bit.testBit(i);
            v <<= 1;
            v += b;

            x += b ? "░" : "█";
        }
        stream << uppercasedigits << hex << v << 'U';
    }

    return x.data();
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

    CHAR_INFO charInfo = { fstRow, static_cast<const quint8>(lstRow - fstRow + 1) ,
                           static_cast<const quint8>(image.width()),  positionInBitmap
                         };
    positionInBitmap += size;

    return charInfo;
}

// ###########################################################

QByteArray GrayscaleColor::scanLine(const quint8 index,
                                    const QImage &image) const
{
    const quint8 width = image.width();
    QByteArray bytes;
    for (int n = 0; n != width; ++n)
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
    for (int i = 0; i < byte.size(); ++i)
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
    for (quint8 i = fstRow + 1; i < lstRow; ++i)
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

    CHAR_INFO charInfo = { fstRow, static_cast<const quint8>(lstRow - fstRow + 1),
                           static_cast<const quint8>(image.width()),  positionInBitmap
                         };
    positionInBitmap += size;

    return charInfo;
}
