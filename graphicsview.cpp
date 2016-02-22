#include "graphicsview.h"

GraphicsView::GraphicsView (QWidget* parent) : QGraphicsView (parent)
{
    setRenderHint (QPainter::NonCosmeticDefaultPen);
}


void GraphicsView::mousePressEvent (QMouseEvent* e)
{
}

void GraphicsView::mouseMoveEvent (QMouseEvent* e)
{
    emit smouseMoveEvent (e);
}

void GraphicsView::mouseReleaseEvent (QMouseEvent* e)
{
}
