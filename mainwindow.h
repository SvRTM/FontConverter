#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsSimpleTextItem>
#include <QMouseEvent>
#include "graphicsview.h"
#include<QLabel>
#include <QPixmap>

#include<QItemSelection>

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:
    explicit MainWindow (QWidget* parent = 0);
    ~MainWindow ();

    protected:
    void changeEvent (QEvent* e);

    protected slots:
    void released ();
    void _mouseMoveEvent (QMouseEvent* event);

    void currentRowChanged(const QModelIndex & current, const QModelIndex & previous);
    void onImport();
     void onExit();
        void onExport();

        void onPreferOutline(int state);
        void onPreferDevice(int state);
        void onPreferBitmap(int state);
        void onForceOutline(int state);
        void onPreferDefault(int state);
        void onPreferAntialias(int state);
        void onNoSubpixelAntialias(int state);
        void onNoAntialias(int state);
        void onOpenGLCompatible(int state);
        void onNoFontMerging(int state);

        void onPreferMatch(bool checked);
        void onPreferQuality(bool checked);
        void onForceIntegerMetrics(bool checked);

        void	selectionChanged(const QItemSelection & selected, const QItemSelection & deselected);


    private:
        QFont _font;
     QLabel *fontName;
     QLabel *fontSize;


    QMenu *fileMenu;
        QMenu *editMenu;
        QToolBar *fileBar;
        QToolBar *editBar;


        QAction *importAction;
        QAction *exitAction;
        QAction *exportAction;

        QAction *createAction(QString name, QString shortcut, QString tip);
        QAction *createAction(QString iconPath, QString name,
                              QString shortcut, QString tip);
        void createActions();
        void createMenuBar();
        void createToolBar();
        void createStatusBar();
        void createChBoxStyleStrategy();

        void prepareTable(QFont &font);
        void setStyleStrategy(QFont::StyleStrategy styleStrategy) ;
    private:
    Ui::MainWindow* ui;
    //GraphicsView *graphicsView ;
    QGraphicsSimpleTextItem* textItem;

    QPixmap selectedPixmap;

};

#endif // MAINWINDOW_H
