#include "listCharactersScene.h"

#include <QPainter>
#include <QStaticText>
#include <QGraphicsSceneMouseEvent>

ListCharactersScene::ListCharactersScene(const QFont &font, QObject *pParent)
    : QGraphicsScene(pParent), font(font)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);
    for (int i = 0; i < nHCell * nVCell; ++i)
        cellInfos[i] = CellInfo();
}

void ListCharactersScene::createListCharacters(quint16 firstCharacter,
        quint16 lastCharacter)
{
    this->firstCharacter = firstCharacter;
    this->lastCharacter = lastCharacter;
    deselecte();
}

const QVector<QChar> ListCharactersScene::getSelected()
{
    QVector<QChar> selecteds;
    selecteds.reserve(selectedCells.size());
    foreach (CellInfo * cellInfo, selectedCells)
    {
        selecteds.append(QChar(cellInfo->ch));
        cellInfo->selected = false;
    }
    selectedCells.clear();
    update(sceneRect());
    return selecteds;
}

void ListCharactersScene::deselecte()
{
    foreach (CellInfo * cellInfo, selectedCells)
    {
        cellInfo->selected = false;
    }
    selectedCells.clear();
    update(sceneRect());
}

void ListCharactersScene::drawChar(const CellInfo &cellInfo, const QChar &ch,
                                   const QFontMetrics &fm, QPainter *pPainter)
{
    const QBrush brush = cellInfo.selected ? Qt::yellow : Qt::white;
    pPainter->setBrush(brush);
    pPainter->drawRect(cellInfo.rect);
    const int cx = (cellInfo.rect.width() - fm.width(ch)) / 2 + cellInfo.rect.x();
    const int cy = (cellInfo.rect.height() - fm.height()) / 2 + cellInfo.rect.y();
    pPainter->drawStaticText(cx, cy, QStaticText(ch));
}

void ListCharactersScene::drawBackground(QPainter *painter, const QRectF &rectF)
//void ListCharactersScene::drawForeground(QPainter *painter, const QRectF &rectF)
{
    const QFontMetrics fm(font);

    const QRect rect = rectF.toRect();
    if (rect.width() != width())
        for (int i = 0; i < nHCell * nVCell; ++i)
            if (rect.contains(cellInfos[i].rect.topLeft()))
            {
                painter->setFont(font);
                const CellInfo &cellInfo = cellInfos[i];
                drawChar(cellInfo, QChar(cellInfo.ch), fm, painter);
                return;
            }

    const int gridHSize = rect.width() / nHCell;
    const int gridVSize = rect.height() / nVCell;
    if (gridHSize == 0 || gridVSize == 0)
        return;

    int pxSize = (gridHSize >= gridVSize ? gridVSize : gridHSize) - 2 * padding - 4;
    if (pxSize <= 0)
        pxSize = 1;
    font.setPixelSize(pxSize);
    painter->setFont(font);

    int diffV = rect.height() - nVCell * gridVSize;
    for (int y = padding, n = 0; y < rect.height();)
    {
        int y2 = y + gridVSize;
        if (diffV > 0)
            y2++, diffV--;

        int diffH = rect.width() - nHCell * gridHSize;
        for (int x = padding; x < rect.width(); ++n)
        {
            int x2 = x + gridHSize;
            if (diffH > 0)
                x2++, diffH--;

            CellInfo &cellInfo = cellInfos[n];
            cellInfo.rect = QRect(x, y, (x2 - x) + 1  - 2 * padding , (y2 - y) + 1 - 2 * padding );
            quint16 ch = firstCharacter + n;
            if (isInRange(ch))
            {
                cellInfo.ch = ch;
                const QChar qch  = QChar(ch);
                if (isNull(ch, fm))
                    painter->fillRect(cellInfo.rect, QBrush(Qt::Dense2Pattern));
                else if (isReserved(qch))
                {
                    painter->fillRect(cellInfo.rect, QBrush(Qt::gray));
                    painter->fillRect(cellInfo.rect, QBrush(Qt::BDiagPattern));
                }
                else if (isOtherFormat(ch))
                {
                    painter->fillRect(cellInfo.rect, QBrush(Qt::transparent));
                    painter->fillRect(cellInfo.rect, QBrush(Qt::FDiagPattern));
                }
                else if (isC0_C1(ch))
                {
                    painter->fillRect(cellInfo.rect, QBrush(Qt::lightGray));
                    painter->fillRect(cellInfo.rect, QBrush(Qt::DiagCrossPattern));
                }
                else
                    drawChar(cellInfo, qch, fm, painter);
            }
            else
                painter->fillRect(cellInfo.rect, QBrush(QColor(QRgb(0xFF404040))));
            x = x2;
        }
        y = y2;
    }
}

void ListCharactersScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (Qt::LeftButton != mouseEvent->button())
        return;

    const QPoint pt = mouseEvent->scenePos().toPoint();
    for (int i = 0; i < nHCell * nVCell; ++i)
        if (cellInfos[i].rect.contains(pt))
        {
            const QFontMetrics fm(font);
            CellInfo &cellInfo = cellInfos[i];
            const quint16 ch = cellInfo.ch;
            const QChar qch(ch);
            if ( !isInRange(ch) || isNull(qch, fm) || isReserved(qch) || isOtherFormat(ch)
                    || isC0_C1(ch))
                return;

            isLeftBtnPressed = true;
            cellInfo.selected = !cellInfo.selected;
            isSelected = cellInfo.selected;
            update(cellInfo.rect.x(), cellInfo.rect.y(), cellInfo.rect.width(),
                   cellInfo.rect.height());
            if (isSelected)
                selectedCells.append(&cellInfo);
            return;
        }
}

void ListCharactersScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *)
{
    if (!isLeftBtnPressed)
        return;

    if (!isSelected)
    {
        QLinkedList<CellInfo *>::iterator itr = selectedCells.begin();
        while (itr != selectedCells.end())
            if (! (*itr)->selected)
                itr = selectedCells.erase(itr);
            else
                ++itr;
    }
    isLeftBtnPressed = false;
}

void ListCharactersScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    if (!isLeftBtnPressed)
        return;

    const QPoint pt = mouseEvent->scenePos().toPoint();
    for (int i = 0; i < nHCell * nVCell; ++i)
        if (cellInfos[i].rect.contains(pt))
        {
            const QFontMetrics fm(font);
            CellInfo &cellInfo = cellInfos[i];
            const quint16 ch = cellInfo.ch;
            const QChar qch(ch);
            if (cellInfo.selected == isSelected || !isInRange(ch) || isNull(qch, fm)
                    || isReserved(qch)
                    || isOtherFormat(ch) || isC0_C1(ch))
                return;

            cellInfo.selected = isSelected;
            update(cellInfo.rect.x(), cellInfo.rect.y(), cellInfo.rect.width(),
                   cellInfo.rect.height());
            if (isSelected)
                selectedCells.append(&cellInfo);
            return;
        }
}
