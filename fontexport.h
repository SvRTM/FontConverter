#ifndef FONTEXPORT_H
#define FONTEXPORT_H

#include "tableitem.h"

#include <QList>
#include <QTextStream>
#include <QBitArray>
#include <QImage>


/*
struct ONE_BIT_COLOR
{
    const __uint8_t   height        = 16;
    const __uint16_t firstSymbol = 65;
    const struct CHAR_INFO
    {
        const __uint8_t   fstRow;
        const __uint8_t   lstRow;
        const __uint8_t   width;
        const __uint16_t position;
    } descriptors[123] = {...};

    const __uint8_t bitmaps[456] = {...};
};

// with blocks:
struct ONE_BIT_COLOR
{
    const __uint8_t   height    = 16;
    const struct CHAR_INFO
    {
        const __uint8_t   fstRow;
        const __uint8_t   lstRow;
        const __uint8_t   width;
        const __uint16_t position;
    } descriptors[123] = {...};

    const struct BLOCK
    {
        const __uint16_t    startChar;
        const __uint16_t    endChar;
        const CHAR_INFO *descriptors;
    } blocks[12] = {...};

    const __uint8_t bitmaps[456] = {..};
};
*/

class IFontExport
{
    protected:
        struct CHAR_INFO
        {
            const quint8 fstRow;
            const quint8 lstRow;
            const quint8 width;
            const quint16 position;
        };

    public:
        explicit IFontExport(const QList<TableItem *> &items);
        virtual ~IFontExport(){}

        QString process();

    private:
        virtual CHAR_INFO prepareBitmaps_CharInfo(const QImage &image, QTextStream &bitmaps) = 0;

        void prepareCharInfo(const QChar ch, const CHAR_INFO &smb, QTextStream &stream)
        {
            stream << '{' << smb.fstRow << ", " << smb.lstRow << ", " << smb.width << ", "
                   << smb.position << "}\t// '" << ch << '\'';
        }


    protected:
        const QList<TableItem *> *items;
        QImage::Format format;

        quint16 positionInBitmap;
        quint16 sizeBitmap;
};

class BitColor final : public IFontExport
{
    public:
        explicit BitColor(const QList<TableItem *> &items) : IFontExport(items)
        {
            format = QImage::Format_Mono;
        }

    private:
        QBitArray scanLine(const quint8 index, const QImage &image) const;

        quint8 firstRow(const QImage &image, QBitArray &bits, bool &empty);
        quint8 lastRow(const QImage &image, QBitArray &bits, bool &empty);

        QString toString(const QBitArray &bit, QTextStream &stream);
        virtual CHAR_INFO prepareBitmaps_CharInfo(const QImage &image,
                                        QTextStream &bitmaps) override;
};

#endif // FONTEXPORT_H
