#ifndef FONTEXPORT_H
#define FONTEXPORT_H

#include "../Widgets/symboltableitem.h"

#include <QList>
#include <QTextStream>
#include <QBitArray>
#include <QByteArray>
#include <QImage>


/*
/#############################/
/#                 V A R I A N T    I             #
/#############################/
struct Tinos_11pt_Italic
{
    const uint8_t    height        = 16;

    const struct CHAR_INFO
    {
        const uint8_t   fstRow;
        const uint8_t   sizeRow;
        const uint8_t   width;
        const uint8_t position;
    } descriptors[1] = {
             {2, 12, 12, 0}		// '￿'	[65535]
       };

    const struct BLOCK
    {
        const uint16_t    startChar;
        const uint16_t    endChar;
        const CHAR_INFO *descriptors;
    } blocks[1] = {
             {65535, 65535, &descriptors[0]}
       };

    const uint8_t bitmaps[24] = {...};
};


/#############################/
/#                 V A R I A N T    II            #
/#############################/
struct IFont
{
    enum class Mode
    {
        Antialias,
        Bitmap
    };

    explicit IFont(uint8_t _height, uint8_t _sizeOfBlock, Mode _mode)
        : height(_height), sizeOfBlock(_sizeOfBlock), mode(_mode) {}

    struct CHAR_INFO
    {
        const uint8_t   fstRow;
        const uint8_t   sizeRow;
        const uint8_t   width;
        const uint16_t position;
    };
    struct BLOCK
    {
        const uint16_t    startChar;
        const uint16_t    endChar;
        const CHAR_INFO *descriptors;
    };

    const uint8_t height;
    const uint8_t sizeOfBlock;
    const Mode   mode;

    virtual const BLOCK *blocks()    const = 0;
    virtual const uint8_t *bitmaps() const = 0;

    static inline const IFont::CHAR_INFO *const descriptor(const wchar_t ch, const IFont &font);
};

const IFont::CHAR_INFO *const IFont::descriptor(const wchar_t ch, const IFont &font)
{
    for (size_t n = 0; n < font.sizeOfBlock; ++n)
    {
        const IFont::BLOCK *const block = &font.blocks()[n];
        if (ch >= block->startChar && ch <= block->endChar)
            return &block->descriptors[ch - block->startChar];
    }

    const IFont::BLOCK *const block = &font.blocks()[font.sizeOfBlock - 1];
    return &block->descriptors[0];
}
# # # # # # # # # # # # # # # # #
struct Tinos_11pt_Italic : IFont
{
    Tinos_11pt_Italic() : IFont(16, 1, Mode::Bitmap) {}

    const CHAR_INFO descriptors[1] = {
             {2, 12, 12, 0}		// '￿'	[65535]
    };
    const BLOCK _blocks[1] = {
             {65535, 65535, &descriptors[0]}
    };
    const uint8_t _bitmaps[24] = {...};

    const IFont::BLOCK *blocks() const override
    {
        return _blocks;
    }
    const uint8_t *bitmaps() const override
    {
        return _bitmaps;
    }
};
*/

enum class FontMode
{
    Antialias,
    Bitmap
} ;

class IFontExport
{
    public:
        enum class CxxStandart
        {
            Cxx11,
            Cxx14
        };
        enum class Mode
        {
            M1,
            M2
        };

    protected:
        struct CHAR_INFO
        {
            const quint8 fstRow;
            const quint8 sizeRow;
            const quint8 width;
            const quint32 position;
        };

    public:
        explicit IFontExport(const QString &fontName, QList<SymbolTableItem *> items,
                             const QImage::Format _format, const Mode _mode,
                             const FontMode _fontMode);
        virtual ~IFontExport() {}

        QString process();
        static const char *saveIFont() ;

    private:
        virtual CHAR_INFO prepareBitmaps_CharInfo(const QImage &image,
                                                  QTextStream &bitmaps) = 0;

        void prepareCharInfo(const QChar ch, const CHAR_INFO &smb, QTextStream &stream,
                             bool latch) const
        {
            if (latch)
                stream << endl << "            ,";
            stream << '{' << smb.fstRow << ", " << smb.sizeRow << ", " << smb.width << ", "
                   << smb.position << "}\t\t// '" << ch << "\'\t[" << ch.unicode() << ']';
        }

        void prepareBlock( quint16 startChar, quint16 endChar, quint16 nDescriptor,
                           QTextStream &stream, bool c = true) const
        {
            stream << "             {" << startChar << ", " << endChar << ',' <<
                   " &descriptors[" <<  nDescriptor << "]}";
            if (c)
                stream << ',' << endl;
        }

        QString size_t_PosBimap(quint32 size) const
        {
            if (size <= UINT8_MAX)
                return "uint8_t";
            else if (size <= UINT16_MAX )
                return "uint16_t";
            else
                return "uint32_t";
        }

    protected:
        const QString &fontName;
        QList<SymbolTableItem *> items;
        const QImage::Format format;
        const Mode mode;
        const FontMode fontMode;

        quint32 positionInBitmap;
        quint32 sizeBitmap;
};

class BitColor final : public IFontExport
{
    public:
        explicit BitColor(const QString &fontName, QList<SymbolTableItem *> items,
                          const Mode _mode, const CxxStandart _cxx)
            : IFontExport(fontName, items, QImage::Format_Mono, _mode, FontMode::Bitmap),
              cxx(_cxx)
        {
        }

    private:
        QBitArray scanLine(const quint8 index, const QImage &image) const;

        quint8 firstRow(const QImage &image, QBitArray &bits);
        quint8 lastRow(const QImage &image, QBitArray &bits, bool &empty);

        QString toString(const QBitArray &bit, QTextStream &stream);
        quint8 sizeInByte(const QBitArray &bit) const
        {
            return bit.size() / 8 + (bit.size() % 8 ? 1 : 0);
        }

        virtual CHAR_INFO prepareBitmaps_CharInfo(const QImage &image,
                                                  QTextStream &bitmaps) override;
    private:
        const CxxStandart cxx;
};

class GrayscaleColor : public IFontExport
{
    public:
        explicit GrayscaleColor(const QString &fontName, QList<SymbolTableItem *> items,
                                const Mode _mode)
            : IFontExport(fontName, items, QImage::Format_Grayscale8, _mode,
                          FontMode::Antialias)
        {
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
