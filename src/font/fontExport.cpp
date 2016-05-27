/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

#include "fontExport.h"

const char *IFontExport::saveIFont()
{
    return
        "#ifndef IFONT_H_\n"
        "#define IFONT_H_\n"
        "\n"
        "#include <stdint.h>\n"
        "#include <cstddef>\n"
        "\n"
        "struct IFont\n"
        "{\n"
        "    enum class Mode\n"
        "    {\n"
        "        Antialias,\n"
        "        Bitmap\n"
        "    };\n"
        "\n"
        "\n"
        "    explicit IFont(uint8_t _height, uint8_t _sizeOfBlock, Mode _mode)\n"
        "        : height(_height), sizeOfBlock(_sizeOfBlock), mode(_mode) {}\n"
        "    virtual ~IFont() = default;\n"
        "\n"
        "\n"
        "    struct CHAR_INFO\n"
        "    {\n"
        "        const uint8_t   fstRow;\n"
        "        const uint8_t   sizeRow;\n"
        "        const uint8_t   width;\n"
        "        const uint16_t position;\n"
        "    };\n"
        "    struct BLOCK\n"
        "    {\n"
        "        const uint16_t    startChar;\n"
        "        const uint16_t    endChar;\n"
        "        const CHAR_INFO *descriptors;\n"
        "    };\n"
        "\n"
        "    const uint8_t height;\n"
        "    const uint8_t sizeOfBlock;\n"
        "    const Mode   mode;\n"
        "\n"
        "    virtual const BLOCK *blocks()    const = 0;\n"
        "    virtual const uint8_t *bitmaps() const = 0;\n"
        "\n"
        "    static inline const IFont::CHAR_INFO *descriptor(const wchar_t ch, const IFont &font);\n"
        "};\n"
        "\n"
        "const IFont::CHAR_INFO *IFont::descriptor(const wchar_t ch, const IFont &font)\n"
        "{\n"
        "    for (size_t n = 0; n < font.sizeOfBlock; ++n)\n"
        "    {\n"
        "        const IFont::BLOCK *const block = &font.blocks()[n];\n"
        "        if (ch >= block->startChar && ch <= block->endChar)\n"
        "            return &block->descriptors[ch - block->startChar];\n"
        "    }\n"
        "\n"
        "    const IFont::BLOCK *const block = &font.blocks()[font.sizeOfBlock - 1];\n"
        "    return &block->descriptors[0];\n"
        "}\n"
        "\n"
        "#endif /* IFONT_H_ */"
        "\n";
}


IFontExport::IFontExport(const QString &fontName, QList<CharacterInfoTableItem *> items,
                         const QImage::Format format, const Mode mode, const FontMode fontMode)
    : fontName(fontName),
      items(items), format(format), mode(mode), fontMode(fontMode),
      positionInBitmap(0), sizeBitmap(0)
{
}

QPair<QString, QString> IFontExport::process()
{
    QString strBitmap, strSymbol, strBlock;
    QTextStream streamBitmap(&strBitmap), streamSymbol(&strSymbol), streamBlock(&strBlock);

    std::sort(items.begin(), items.end(), []
              (const CharacterInfoTableItem * const a, const CharacterInfoTableItem * const b) -> bool
    {
        return a->numericUnicode() < b->numericUnicode();
    });

    quint8 height = 0;
    quint16 startChar = 0, prevChar = 0, nDescriptor = 0, sizeBlocks = 0;
    bool latch = false;
    for (int n = 0; n < items.size(); ++n)
    {
        CharacterInfoTableItem *const pCharInfo = items[n];
        QPixmap pixmap = pCharInfo->charPixmap().value<QPixmap>();
        QImage image;
        if (!pixmap.isNull())
            image = pixmap.toImage().convertToFormat(format, Qt::AvoidDither);

        quint16 currChar = pCharInfo->numericUnicode();

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

    QString upFName = fontName.toUpper();
    QString hStr, cppStr;
    QTextStream hFile(&hStr), cppFile(&cppStr);
    cppFile << "#include \"" << fontName << ".h\"\n"
            "\n";
    hFile << "#ifndef " << upFName << "_H_\n"
          "#define " << upFName << "_H_\n"
          "\n";

    if (Mode::M1 == mode)
    {
        cppFile << ""
                "constexpr " << fontName << "::CHAR_INFO " << fontName << "::descriptors[" << items.size()
                << "];\n"
                "constexpr " << fontName << "::BLOCK " << fontName << "::blocks[" << sizeBlocks + 1 <<
                "];\n"
                "constexpr uint8_t " << fontName << "::bitmaps[" << sizeBitmap << "];\n";

        hFile  << "#include <stdint.h>\n"
               "\n"
               "\n"
               "/*\n"
               "#include <cstddef>\n"
               "const <FONT>::CHAR_INFO *const descriptor(const wchar_t ch, const <FONT> &font)\n"
               "{\n"
               "    const uint8_t sizeOfBlock = sizeof(<FONT>::blocks) / sizeof(<FONT>::blocks[0]);\n"
               "    for (size_t n = 0; n < sizeOfBlock; ++n)\n"
               "    {\n"
               "        const <FONT>::BLOCK *const block = &font.blocks[n];\n"
               "        if (ch >= block->startChar && ch <= block->endChar)\n"
               "            return &block->descriptors[ch - block->startChar];\n"
               "    }\n"
               "\n"
               "    const <FONT>::BLOCK *const block = &font.blocks[sizeOfBlock - 1];\n"
               "    return &block->descriptors[0];\n"
               "}\n"
               "*/\n"
               "\n"
               "struct " << fontName << "\n"
               "{\n"
               "    static constexpr uint8_t    height        = " << height << ";\n"
               "\n"
               "    static constexpr struct CHAR_INFO\n"
               "    {\n"
               "        const uint8_t   fstRow;\n"
               "        const uint8_t   sizeRow;\n"
               "        const uint8_t   width;\n"
               "        const " << size_t_PosBimap(sizeBitmap) << " position;\n"
               "    } descriptors[" << items.size() << "] = {\n"
               "             " << strSymbol << "\n"
               "       };\n"
               "\n"
               "    static constexpr struct BLOCK\n"
               "    {\n"
               "        const uint16_t    startChar;\n"
               "        const uint16_t    endChar;\n"
               "        const CHAR_INFO *descriptors;\n"
               "    } blocks[" << sizeBlocks + 1 << "] = {\n" <<
               strBlock << "\n"
               "       };\n"
               "\n"
               "    static constexpr uint8_t bitmaps[" << sizeBitmap << "] = {\n" <<
               strBitmap << "\n"
               "    };\n"
               "};";
    }
    /* ################################### */
    else // Mode::M2
    {
        cppFile << ""
                "constexpr IFont::CHAR_INFO " << fontName << "::descriptors[" << items.size() << "];\n"
                "constexpr IFont::BLOCK " << fontName << "::_blocks[" << sizeBlocks + 1 << "];\n"
                "constexpr uint8_t " << fontName << "::_bitmaps[" << sizeBitmap << "];\n";

        hFile << "#include \"ifont.h\"\n"
              "\n"
              "struct " << fontName << " : IFont\n"
              "{\n"
              "    " << fontName << "() : IFont(" << height << ", " << sizeBlocks + 1 <<
              ", Mode::" << (FontMode::Bitmap == fontMode ? "Bitmap" :  "Antialias") <<
              ") {}\n"
              "\n"
              "    static constexpr CHAR_INFO descriptors[" << items.size() << "] = {\n"
              "             " << strSymbol << "\n"
              "    };\n"
              "    static constexpr BLOCK _blocks[" << sizeBlocks + 1 << "] = {\n" <<
              strBlock << "\n"
              "    };\n"
              "    static constexpr uint8_t _bitmaps[" << sizeBitmap << "] = {\n" <<
              strBitmap << "\n"
              "    };\n"
              "\n"
              "    const IFont::BLOCK *blocks() const override\n"
              "    {\n"
              "        return _blocks;\n"
              "    }\n"
              "    const uint8_t *bitmaps() const override\n"
              "    {\n"
              "        return _bitmaps;\n"
              "    }\n"
              "};\n";
    }
    cppFile << "\n";
    hFile  << "\n"
           "\n"
           "#endif /* " << fontName << "_H_ */"
           "\n";

    return  qMakePair(cppStr, hStr);
}

// ###########################################################

QBitArray BitColor::scanLine(const quint8 index, const QImage &image) const
{
    const quint8 width = image.width();
    QBitArray bits(width);
    for (int n = 0; n != width; ++n)
    {
        bool px = bool(quint8(image.pixel(n, index)));
        bits.setBit(n, px);
    }
    return bits;
}

quint8 BitColor::firstRow(const QImage &image, QBitArray &bits)
{
    const QBitArray EMPTY_LINE(image.width(), true);

    const quint8 height = image.height() - 1;
    if (height == 255)
        return 0;
    quint8 fstRow = 0;
    do
    {
        bits = scanLine(fstRow, image);
        if (bits != EMPTY_LINE)
            break;
        fstRow++;
    }
    while (fstRow != height);

    return fstRow;
}

quint8 BitColor::lastRow(const QImage &image, QBitArray &bits, bool &empty)
{
    const QBitArray EMPTY_LINE(image.width(), true);

    quint8 lstRow = image.height() - 1;
    if (lstRow == 255)
    {
        empty = true;
        return 0;
    }

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

    if (bit.size() == 0)
        stream << 0;
    else
    {
        if (CxxStandart::Cxx14 == cxx)
            for (int i = 0; i < bit.size(); ++i)
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
            for (int i = 0; i < bit.size(); ++i)
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
    }

    return x.data();
}

IFontExport::CHAR_INFO BitColor::prepareBitmaps_CharInfo(const QImage &image,
        QTextStream &bitmaps)
{
    quint16 size = 0;

    QBitArray fbits;
    const quint8 fstRow = firstRow(image, fbits);
    bitmaps << "    /* " << toString(fbits, bitmaps) << "*/";
    size += sizeInByte(fbits);

    bool latch = false;
    bool isEmpty;
    QBitArray lbits;
    const quint8 lstRow = lastRow(image, lbits, isEmpty);
    for (quint8 i = fstRow + 1; i < lstRow; ++i)
    {
        QBitArray bit = scanLine(i, image);
        if (!latch)
            bitmaps << ',' << endl;
        else
            latch = false;
        bitmaps << "    /* " << toString(bit, bitmaps) << "*/";
        size += sizeInByte(bit);
    }

    if (!isEmpty)
    {
        if (!latch)
            bitmaps << ',' << endl;
        bitmaps << "    /* " << toString(lbits, bitmaps) << "*/";
        size += sizeInByte(lbits);
    }

    sizeBitmap += size;

    CHAR_INFO charInfo = { fstRow, quint8(fstRow == 0 && lstRow == 0 ? 0 : lstRow - fstRow + 1),
                           quint8(image.width()), positionInBitmap
                         };
    positionInBitmap += size;

    return charInfo;
}

// ###########################################################

QByteArray GrayscaleColor::scanLine(const quint8 index, const QImage &image) const
{
    const quint8 width = image.width();
    QByteArray bytes;
    for (int n = 0; n != width; ++n)
    {
        quint8 px = quint8(image.pixel(n, index));
        bytes.append(px);
    }
    return bytes;
}

quint8 GrayscaleColor::firstRow(const QImage &image, QByteArray &bytes)
{
    const QByteArray EMPTY_LINE(image.width(), uint8_t(255));

    const quint8 height = image.height() - 1;
    if (height == 255)
        return 0;
    quint8 fstRow = 0;
    do
    {
        bytes = scanLine(fstRow, image);
        if (bytes != EMPTY_LINE)
            break;
        fstRow++;
    }
    while (fstRow != height);

    return fstRow;
}

quint8 GrayscaleColor::lastRow(const QImage &image, QByteArray &bytes, bool &empty)
{
    const QByteArray EMPTY_LINE(image.width(), uint8_t(255));

    quint8 lstRow = image.height() - 1;
    if (lstRow == 255)
    {
        empty = true;
        return 0;
    }

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
    return image.height() - 1;
}

void GrayscaleColor::toString(const QByteArray &byte, QTextStream &stream)
{
    if (byte.isNull())
        return;

    QString str;
    QTextStream stm(&str);
    for (int i = 0; i < byte.size(); ++i)
    {
        quint8 b = quint8(byte[i]);

        if (i != 0)
            stm << ", " ;
        else
            stream << "/* ";
        stream << QChar(
                   b < 51
                   ?  u'█'
                   : (b >= 51 && b < 102
                      ? u'▓'
                      : (b >= 102 && b < 153
                         ? u'▒'
                         : (b >= 153 && b < 204
                            ? u'░'
                            : u'▁'
                           )
                        )
                     )
               );
        stm << "0x" << uppercasedigits << hex << b;
    }
    stream << " */\t\t" << str;
}

IFontExport::CHAR_INFO GrayscaleColor::prepareBitmaps_CharInfo(const QImage &image,
        QTextStream &bitmaps)
{
    quint16 size = 0;

    QByteArray fbytes;
    const quint8 fstRow = firstRow(image, fbytes);
    toString(fbytes, bitmaps) ;
    size += fbytes.size();

    bool latch = false;
    bool isEmpty;
    QByteArray lbytes;
    const quint8 lstRow = lastRow(image, lbytes, isEmpty);
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

    CHAR_INFO charInfo = { fstRow, quint8(fstRow == 0 && lstRow == 0 ? 0 : lstRow - fstRow + 1),
                           quint8(image.width()), positionInBitmap
                         };
    positionInBitmap += size;

    return charInfo;
}
