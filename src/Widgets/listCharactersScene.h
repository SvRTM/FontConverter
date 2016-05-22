/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

#ifndef LISTCHARACTERSSCENE_H
#define LISTCHARACTERSSCENE_H

#include <QGraphicsScene>
#include <QLinkedList>
#include <QFontMetrics>

#include <array>

class ListCharactersScene final : public QGraphicsScene
{
        Q_OBJECT

        struct CellInfo final
        {
            QRect rect;
            quint16 ch;
            bool selected = false;
        };

    public:
        explicit ListCharactersScene(const QFont &font, QObject *pParent);
        virtual ~ListCharactersScene() {}

    public:
        void createListCharacters(quint16 firstCharacter, quint16 lastCharacter);
        const QVector<QChar> getSelected();

    protected:
        void drawBackground(QPainter *painter, const QRectF &rectF) override;
        //void drawForeground(QPainter *painter, const QRectF &rectF) override;

        void  mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
        void mouseReleaseEvent(QGraphicsSceneMouseEvent *) override;
        void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

    private:
        inline bool isInRange(quint16 i)
        {
            return i >= firstCharacter && i <= lastCharacter;
        }
        inline bool isNull(const QChar &ch, const QFontMetrics &fm)
        {
            return fm.width(ch) == 0;
        }

        inline bool isReserved(const QChar &ch)
        {
            return QChar::Script_Unknown == ch.script();
        }
        inline bool isOtherFormat(const QChar &ch)
        {
            return QChar::Other_Format == ch.category();
        }

        inline bool isC0_C1(quint16 ch)
        {
            return (ch <= 0x001F) || ( ch >= 0x007F && ch <= 0x009F);
        }

        void drawChar(const CellInfo &cellInfo, const QChar &ch, const QFontMetrics &fm,
                      QPainter *pPainter);
        void deselecte();

    private:
        static constexpr quint8 padding = 2;
        static constexpr quint8 nHCell = 16;
        static constexpr quint8 nVCell = 16;

        std::array<CellInfo, 16 * 16> cellInfos;
        bool isLeftBtnPressed = false;
        bool isSelected = false;
        QLinkedList<CellInfo *> selectedCells;

        QFont font;
        quint16 firstCharacter;
        quint16 lastCharacter;
};

#endif // LISTCHARACTERSSCENE_HH
