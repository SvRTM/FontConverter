#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Widgets/TableModel/characterInfoTableModel.h"
#include "Widgets/TableView/characterInfoTableView.h"
#include "Widgets/Tab/selectedCharactersPage.h"
#include "Widgets/Tab/fixedCharactersPage.h"
#include "Dialogs/exportDialog.h"
#include <QFontDialog>
#include <QStaticText>
#include <QPainter>

MainWindow::MainWindow(QWidget *pParent)
    : QMainWindow(pParent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->updateTable->setDisabled(true);
    ui->tabWidget->setDisabled(true);

    QList<int> sizes;
    sizes << 380 << 768;
    ui->splitter->setSizes(sizes);

    prepareToolBar();
    createStatusBar();

    createSelectionModel();

    prepareGraphicsView();

    cleareScene(true);
    setStyleStrategy(QFont::NoAntialias);
}
MainWindow::~MainWindow ()
{
    ui->characterInfoTableView->clearSelection();
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void MainWindow::prepareToolBar() const
{
    ui->mainToolBar->addAction(ui->actionImportFont);
    ui->mainToolBar->addAction(ui->actionExportFontC);
}

void MainWindow::createStatusBar()
{
    QLabel *pLbl = new QLabel("Font name: ", statusBar());
    pLbl->setAlignment(Qt::AlignLeading);
    statusBar()->addWidget(pLbl);
    m_StatusBar.pFontName = new QLabel(statusBar());
    statusBar()->addWidget(m_StatusBar.pFontName);

    statusBar()->addWidget(separator());

    pLbl = new QLabel("Font size: ", statusBar());
    pLbl->setAlignment(Qt::AlignLeading);
    statusBar()->addWidget(pLbl);
    m_StatusBar.pFontSize = new QLabel(statusBar());
    statusBar()->addWidget(m_StatusBar.pFontSize);

    statusBar()->addWidget(separator());

    statusBar()->addWidget(new QLabel("Font style:", statusBar()));

    m_StatusBar.pFontStyleName = new QLabel(statusBar());
    m_StatusBar.pFontStyleName->setAlignment(Qt::AlignLeading);
    statusBar()->addWidget(m_StatusBar.pFontStyleName);

    statusBar()->addWidget(separator());

    m_StatusBar.pFontStyleStrategy = new QLabel("Antialias", statusBar());
    m_StatusBar.pFontStyleStrategy->setAlignment(Qt::AlignLeading);
    m_StatusBar.pFontStyleStrategy->setVisible(false);
    statusBar()->addWidget(m_StatusBar.pFontStyleStrategy);
}

void MainWindow::createSelectionModel() const
{
    QAbstractItemModel *pModel = ui->characterInfoTableView->model();
    QItemSelectionModel *pSelModel = new QItemSelectionModel(pModel);
    ui->characterInfoTableView->setSelectionModel(pSelModel);
    connect(pSelModel, &QItemSelectionModel::selectionChanged,
            this, &MainWindow::tableSelection);
}
void MainWindow::tableSelection(const QItemSelection &selected,
                                const QItemSelection &deselected)
{
    ui->graphicsView->scene()->clear();

    int row;
    if (!selected.isEmpty())
        row = selected.indexes()[0].row();
    else if (!deselected.isEmpty())
        row = deselected.indexes()[0].row();
    else
    {
        ui->characterPropWidget->drawSize();
        return;
    }

    CharacterPropWidget *pChProp = ui->characterPropWidget;
    QPair<int, int> size = viewSymbol(row, pChProp->zoom());
    pChProp->drawSize(size.first, size.second);
}

void MainWindow::prepareGraphicsView()
{
    QGraphicsView *pGraphView = ui->graphicsView;
    pGraphView->setAlignment(Qt::AlignCenter);
    pGraphView->setScene(new QGraphicsScene(this));
    connect(ui->characterPropWidget, &CharacterPropWidget::valueChanged, [&] (int value)
    {
        ui->graphicsView->scene()->clear();
        int row = ui->characterInfoTableView->selectionModel()->currentIndex().row();
        viewSymbol(row, value);
    });
}
QPair<int, int> MainWindow::viewSymbol(int row, int zoom)
{
    QPair<int, int> size(0, 0);

    QPixmap pixmap = ui->characterInfoTableView->model()->charPixmap(row);
    if (pixmap.isNull())
        return size;

    QImage::Format format = QImage::Format_Grayscale8;
    if (QFont::NoAntialias & m_font.styleStrategy())
        format = QImage::Format_Mono;
    pixmap = QPixmap::fromImage(pixmap.toImage().convertToFormat(format, Qt::AvoidDither));

    size.first = pixmap.width();
    const int width = size.first * zoom;
    size.second = pixmap.height();
    const int height = size.second * zoom;
    QGraphicsScene *pGraphScene = ui->graphicsView->scene();
    pGraphScene->setSceneRect(0, 0, width, height);
    if (!pixmap.isNull())
        pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio);

    if (zoom > 3)
    {
        QPainter painter(&pixmap);
        painter.setPen(Qt::gray);

        QVarLengthArray<QLine> lines;
        lines.reserve((width + height) / zoom - 2);
        for (int n = zoom; n < width; n += zoom)
            lines.append(QLine(n, 0, n, height));
        for (int n = zoom; n < height; n += zoom)
            lines.append(QLine(0, n, width, n));
        painter.drawLines(lines.data(), lines.size());
    }
    pGraphScene->addPixmap(pixmap);

    return size;
}


// #################################################################
// #################################################################

void MainWindow::on_tabWidget_currentChanged(int)
{
    prepareTable(m_font);
}

void MainWindow::on_updateTable_clicked()
{
    prepareTable(m_font);
}

void MainWindow::prepareTable(QFont &font)
{
    ICharacters *pIChars = qobject_cast<ICharacters *>(ui->tabWidget->currentWidget());
    const QString chars = pIChars->getCharacters();

    CharacterInfoTableView *pCharInfoTableView = ui->characterInfoTableView;

    if (chars.isEmpty())
    {
        pCharInfoTableView->model()->clear();
        ui->characterPropWidget->drawSize();
        cleareScene(true);
        return;
    }

    int selRow = -1;
    QItemSelectionModel *pSelModel = pCharInfoTableView->selectionModel();
    if (pSelModel->hasSelection())
        selRow = pSelModel->selectedRows()[0].row();
    cleareScene(false);

    m_StatusBar.pFontStyleName->setText(font.styleName());
    m_StatusBar.pFontName->setText(font.family());
    m_StatusBar.pFontSize->setText(QString ("%1").arg(font.pointSize()));

    QFontMetrics fm(font);
    int height = fm.height();

    pCharInfoTableView->setRowCount(chars.size());

    int rowCount = 0;
    foreach (const QChar ch, chars)
    {
        int width = fm.width(ch);

        QPixmap pixmap(width, height);
        if (width != 0)
        {
            QPainter painter(&pixmap);
            painter.setFont(font);
            painter.fillRect(0, 0, width, height, Qt::white);
            if (ch == FixedCharactersPage::UnknownSymbol)
                drawUnknowSymbol(painter, height, width);
            else
                painter.drawStaticText(0, 0, QStaticText(ch));
        }

        pCharInfoTableView->setItem(rowCount++, new CharacterInfoTableItem(ch.unicode(),
                                    pixmap));
    }
    pCharInfoTableView->resizeColumnsToContents();
    pCharInfoTableView->resizeRowsToContents();

    emit pCharInfoTableView->selectRow(selRow);

    ui->updateTable->setDisabled(false);
    ui->tabWidget->setDisabled(false);
}

void MainWindow::drawUnknowSymbol(QPainter &painter, int height, int width)
{
    const int c = (height - width) / 2;
    painter.fillRect(0, c, width, width, Qt::black);
    painter.setPen(Qt::white);
    painter.drawLine(1, c + 1, width - 1 - 1, width - 1 + c - 1);
    painter.drawLine(1, width - 1 + c - 1, width - 1 - 1, c + 1);
}

void MainWindow::setStyleStrategy(QFont::StyleStrategy styleStrategy)
{
    m_font.setStyleStrategy((QFont::StyleStrategy) (m_font.styleStrategy() ^ styleStrategy));
}

void MainWindow::cleareScene(bool cleare) const
{
    ui->characterInfoTableView->clearSelection();
    ui->graphicsView->scene()->clear();

    ui->actionExportFontC->setEnabled(!cleare);
    ui->actionNoAntialias->setEnabled(!cleare);
}

void MainWindow::on_actionImportFont_triggered()
{
    QFont::StyleStrategy styleStrategy = m_font.styleStrategy();
    bool ok;
    m_font = QFontDialog::getFont(&ok, m_font, this);
    m_font.setStyleStrategy(styleStrategy);
    if (!ok)
        return;

    SelectedCharactersPage *pSelChars = qobject_cast<SelectedCharactersPage *>
                                        (ui->tabWidget->widget(1));
    pSelChars->setFont(m_font);
    prepareTable(m_font);
}

void MainWindow::on_actionExportFontC_triggered()
{
    FontInfo fontInfo
    {
        m_font.styleStrategy() &QFont::NoAntialias ? FontMode::Bitmap : FontMode::Antialias,
        m_font.family(), m_font.pointSize(), m_font.styleName()
    };

    ExportDialog exp(fontInfo, ui->characterInfoTableView->items(), this);
    exp.exec();
}

void MainWindow::on_actionNoAntialias_triggered(bool e)
{
    m_StatusBar.pFontStyleStrategy->setVisible(!e);
    setStyleStrategy(QFont::NoAntialias);
    prepareTable(m_font);
}

void MainWindow::on_actionExit_triggered()
{
    close();
}
