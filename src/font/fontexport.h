#ifndef FONTEXPORT_H
#define FONTEXPORT_H

#include "../Widgets/symboltableitem.h"

#include <QList>
#include <QTextStream>
#include <QBitArray>
#include <QByteArray>
#include <QImage>


/*
struct ONE_BIT_COLOR
{
    const uint8_t   height        = 16;
    const uint16_t firstSymbol = 65;
    const struct CHAR_INFO
    {
        const uint8_t   fstRow;
        const uint8_t   lstRow;
        const uint8_t   width;
        const uint16_t position;
    } descriptors[123] = {...};

    const uint8_t bitmaps[456] = {...};
};

struct IFONT
{
    const struct CHAR_INFO
    {
        const uint8_t   fstRow;
        const uint8_t   lstRow;
        const uint8_t   width;
        const uint16_t position;
    }
}

// with blocks:
struct ONE_BIT_COLOR
{
    const uint8_t   height    = 16;
    const struct CHAR_INFO
    {
        const uint8_t   fstRow;
        const uint8_t   lstRow;
        const uint8_t   width;
        const uint16_t position;
    } descriptors[123] = {...};

    const struct BLOCK
    {
        const uint16_t    startChar;
        const uint16_t    endChar;
        const CHAR_INFO *descriptors;
    } blocks[12] = {...};

    const uint8_t bitmaps[456] = {..};
};
// ///////////////
struct IFONT
{
    struct CHAR_INFO
    {
        const uint8_t   fstRow;
        const uint8_t   lstRow;
        const uint8_t   width;
        const uint16_t position;
    };
    struct BLOCK
    {
        const uint16_t    startChar;
        const uint16_t    endChar;
        const CHAR_INFO *descriptors;
    };

    virtual uint8_t height()           const = 0 ;
    virtual const BLOCK *blocks()  const = 0 ;
};

struct ONE_BIT_COLOR : IFONT
{
    const uint8_t _height = 16;
    const CHAR_INFO descriptors[1] = {{1, 2, 3, 4}};
    const BLOCK _blocks[1] = {{11, 22, &descriptors[0]}};
    const uint8_t bitmaps[3] = {3, 5, 7};

    uint8_t height() const override
    {
        return _height;
    }
    const BLOCK *blocks() const override
    {
        return _blocks;
    }
} ;
*/


class IFontExport
{
    protected:
        struct CHAR_INFO
        {
            const quint8 fstRow;
            const quint8 sizeRow;
            const quint8 width;
            const quint32 position;
        };

    public:
        explicit IFontExport(QList<SymbolTableItem *> items);
        virtual ~IFontExport() {}

        QString process();

    private:
        virtual CHAR_INFO prepareBitmaps_CharInfo(const QImage &image,
                                                  QTextStream &bitmaps) = 0;

        void prepareCharInfo(const QChar ch, const CHAR_INFO &smb, QTextStream &stream,
                             bool latch)
        {
            if (latch)
                stream << endl << "            ,";
            stream << '{' << smb.fstRow << ", " << smb.sizeRow << ", " << smb.width << ", "
                   << smb.position << "}\t\t// '" << ch << "\'\t[" << ch.unicode() << ']';
        }

        void prepareBlock( quint16 startChar, quint16 endChar, quint16 nDescriptor,
                           QTextStream &stream, bool c = true)
        {
            stream << "             {" << startChar << ", " << endChar << ',' <<
                   " &descriptors[" <<  nDescriptor << "]}";
            if (c)
                stream << ',' << endl;
        }

        QString size_t_PosBimap(quint32 size)
        {
            if (size <= UINT8_MAX)
                return "uint8_t";
            else if (size <= UINT16_MAX )
                return "uint16_t";
            else
                return "uint32_t";
        }

    protected:
        QList<SymbolTableItem *> items;
        QImage::Format format;

        quint32 positionInBitmap;
        quint32 sizeBitmap;
};

class BitColor final : public IFontExport
{
    public:
        explicit BitColor(QList<SymbolTableItem *> items) : IFontExport(items)
        {
            format = QImage::Format_Mono;
        }

    private:
        QBitArray scanLine(const quint8 index, const QImage &image) const;

        quint8 firstRow(const QImage &image, QBitArray &bits);
        quint8 lastRow(const QImage &image, QBitArray &bits, bool &empty);

        QString toString(const QBitArray &bit, QTextStream &stream);
        quint8 sizeInByte(const QBitArray &bit)
        {
            return bit.size() / 8 + (bit.size() % 8 ? 1 : 0);
        }

        virtual CHAR_INFO prepareBitmaps_CharInfo(const QImage &image,
                                                  QTextStream &bitmaps) override;
};

class GrayscaleColor : public IFontExport
{
    public:
        explicit GrayscaleColor(QList<SymbolTableItem *> items) : IFontExport(items)
        {
            format = QImage::Format_Grayscale8;
        }

    private:
        QByteArray scanLine(const quint8 index, const QImage &image) const;

        quint8 firstRow(const QImage &image, QByteArray &bytes);
        quint8 lastRow(const QImage &image, QByteArray &bytes, bool &empty);

        void toString(const QByteArray &byte, QTextStream &stream);

        virtual CHAR_INFO prepareBitmaps_CharInfo(const QImage &image,
                                                  QTextStream &bitmaps) override;
};


#endif // FONTEXPORT_H
