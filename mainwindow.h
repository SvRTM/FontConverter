#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "tableitem.h"

#include <QMainWindow>
#include <QGraphicsSimpleTextItem>

#include <QLabel>
#include <QPixmap>

#include <QItemSelection>
#include <QStandardItem>

#include <QTextStream>
#include <QBitArray>

#include <unordered_map>


namespace Ui
{
    class MainWindow;
}

struct CodeNumbers final : std::unordered_map<int, QString>
{
    CodeNumbers();
    virtual ~CodeNumbers() {}

    enum eCode
    {
        LatinLetters = 0,
        CyrillicLetters,
        Numbers,
        LatinSupplement,
        Arrows,
        BoxDrawing,
        BlockElements,
        GeometricShapes,
        MiscellanepousSymbols
    };
};

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = 0);
        virtual ~MainWindow();

    protected:
        void changeEvent(QEvent *e);

    private:
        void prepareCodeNumbersList(QStandardItemModel *listModel);
        QStandardItem *createCodeListItem(QString text,
                                           Qt::CheckState state = Qt::Checked);

        QAction *createAction(QString name, QString shortcut, QString tip);
        QAction *createAction(QString iconPath, QString name, QString shortcut,
                               QString tip);

        void createActions();
        void createMenuBar();
        void createToolBar();
        void createStatusBar();
        void createChBoxStyleStrategy();

        void prepareTable(QFont &font);
        void setStyleStrategy(QFont::StyleStrategy styleStrategy);


    private slots:
        void selectFont();

        void tableSelection(const QItemSelection &selected,
                             const QItemSelection &deselected);

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


    private:
        Ui::MainWindow *ui;

        CodeNumbers m_codeNumbers;

        QPixmap selectedPixmap;

        QFont m_font;
        QLabel *fontName;
        QLabel *fontSize;

        QMenu *fileMenu;
        QMenu *editMenu;
        QToolBar *fileBar;
        QToolBar *editBar;

        QAction *importAction;
        QAction *exitAction;
        QAction *exportAction;
};

#endif // MAINWINDOW_H
