#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QObject>
#include <QGraphicsView>
#include <QMouseEvent>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

    public:
    GraphicsView (QWidget* parent);

    virtual void mousePressEvent (QMouseEvent*);
    virtual void mouseMoveEvent (QMouseEvent*);
    virtual void mouseReleaseEvent (QMouseEvent*);

    signals:
    void smouseMoveEvent (QMouseEvent*);
};

#endif // GRAPHICSSCENE_H
