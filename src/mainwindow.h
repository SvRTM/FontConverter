#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Widgets/symboltableitem.h"

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
        void prepareToolBar();
        void createStatusBar();
        void createSelectionModel() const;

        void prepareGraphicsView();
        QPair<int, int> viewSymbol(int row, int zoom);

        void prepareListCharacters();
        void prepareCodeNumbersList(QStandardItemModel *listModel);
        QStandardItem *createCodeListItem(QString txt, Qt::CheckState state = Qt::Checked);

        void prepareTable(QFont &font);
        void drawUnknowSymbol(QPainter &painter, int height, int width);
        void setStyleStrategy(QFont::StyleStrategy styleStrategy);

        void cleareScene(bool cleare);

        QFrame *separator()
        {
            QFrame *line = new QFrame(this);
            line->setFrameShape(QFrame::VLine);
            line->setFrameShadow(QFrame::Sunken);
            return line;
        }

    private slots:
        void tableSelection(const QItemSelection &selected,
                            const QItemSelection &deselected);

        void on_updateTbl_clicked();

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
        QLabel *sbFontStyleName;
};

#endif // MAINWINDOW_H
