/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Widgets/characterInfoTableItem.h"

#include <QMainWindow>
#include <QItemSelection>
#include <QLabel>

namespace Ui
{
class MainWindow;
}

class MainWindow final : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *pParent = nullptr);
        virtual ~MainWindow();

    protected:
        void changeEvent(QEvent *e);

    private:
        void prepareToolBar() const;
        void createStatusBar();
        void createSelectionModel() const;

        void prepareGraphicsView();
        QPair<int, int> viewSymbol(int row, int zoom);


        void prepareTable();
        void drawUnknowSymbol(QPainter &painter, int height, int width);
        void setStyleStrategy(QFont::StyleStrategy styleStrategy);

        void cleareScene(bool cleare) const;

        QFrame *separator()
        {
            QFrame *pLine = new QFrame(this);
            pLine->setFrameShape(QFrame::VLine);
            pLine->setFrameShadow(QFrame::Sunken);
            return pLine;
        }

    private slots:
        void tableSelection(const QItemSelection &selected, const QItemSelection &deselected);

        void on_tabWidget_currentChanged(int);

        void on_updateTable_clicked();

        void on_actionImportFont_triggered();
        void on_actionExportFontC_triggered();
        void on_actionNoAntialias_triggered(bool e);

        void on_actionExit_triggered();

    private:
        Ui::MainWindow *const ui;

        QFont m_font;

        struct StatusBar final
        {
            QLabel *pFontName;
            QLabel *pFontSize;
            QLabel *pFontStyleStrategy;
            QLabel *pFontStyleName;
        } m_StatusBar;
};

#endif // MAINWINDOW_H
