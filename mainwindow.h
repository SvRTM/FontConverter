#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "symboltableitem.h"

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

struct CodeNumbers final : std::unordered_map<quint8, QString>
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
        MiscellanepousSymbols,
        UnknownSymbol
    };
};

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        virtual ~MainWindow();

    protected:
        void changeEvent(QEvent *e);

    private:
        void prepareCodeNumbersList(QStandardItemModel *listModel);
        QStandardItem *createCodeListItem(QString txt, Qt::CheckState state = Qt::Checked);

        void createToolBar();
        void createStatusBar();

        void prepareTable(QFont &font);
        void setStyleStrategy(QFont::StyleStrategy styleStrategy);

        void disableAction(bool disable);
        QFrame *separator()
        {
            QFrame *line = new QFrame(this);
            line->setFrameShape(QFrame::VLine);
            line->setFrameShadow(QFrame::Sunken);
            return line;
        }

    private slots:
        void tableSelection(const QItemSelection &selected, const QItemSelection &deselected);

        void on_actionImportFont_triggered();
        void on_actionExportFontC_triggered();
        void on_actionNoAntialias_triggered(bool e);

        void on_actionExit_triggered();

    private:
        Ui::MainWindow *const ui;

        CodeNumbers m_codeNumbers;

        QFont m_font;

        QLabel *sbFontName;
        QLabel *sbFontSize;
        QLabel *sbFontStyleStrategy;
};

#endif // MAINWINDOW_H
