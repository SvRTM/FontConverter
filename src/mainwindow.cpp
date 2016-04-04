#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Widgets/TableModel/symboltablemodel.h"
#include "Widgets/TableView/symboltableview.h"
#include "Dialogs/exportdialog.h"

#include <QFontDialog>
#include <QStaticText>
#include <QBitmap>


CodeNumbers::CodeNumbers ()
{
    operator[](LatinLetters) = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"  /* [65 - 90] */
                               "abcdefghijklmnopqrstuvwxyz"; /* [97 - 122] */
    operator[](CyrillicLetters) = "Ё" /* [1025] */
                                  "АБВГДЕЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ" /* [1040 - 1071] */
                                  "абвгде"
                                  "жзийклмнопрстуфхцчшщъыьэюя" /* [1072 - 1103] */
                                  "ё" /* [1105] */;

    operator[](Numbers) = "0123456789";                      /* [48 - 57] */
    operator[](LatinSupplement) = " !\"#$%&'()*+,-./"        /* [32 - 47] */
                                  ":;<=>?@"                                 /* [58 - 64] */
                                  "[\\]^_`"                                     /* [91 - 96] */
                                  "{|}~"                                         /* [123 - 126] */;

    operator[](Arrows) = "←↑→↓↔↕"   /* [8592 - 8597] */
                         "↵"                      /* [8629] */
                         "⇐⇑⇒⇓⇔";            /* [8656 - 8660] */
    operator[](BoxDrawing) =
        "─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬";     /* [9472,9474,9484,9488,9492,9496,9500,9508,9516,9524,9532,9552 - 9580] */
    operator[](BlockElements) =
        "▀▄█▌▐░▒▓";                     /* [ 9616 - 9619 */
    operator[](GeometricShapes) =
        "■▪▫▲►▼◄◊○●";                   /* [9632,9642,9643,9650,9658,9660,9668,9674,9675,9679] */
    operator[](MiscellanepousSymbols) =
        "♠♣♥♦♀♂";                        /* [9824,9827,9829,9830,9792,9794 ] */
    operator[](UnknownSymbol) = QChar(0xFFFF);
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QList<int> sizes;
    sizes << 380 << 768;
    ui->splitter->setSizes(sizes);

    createToolBar();
    createStatusBar();

    createSelectionModel();

    ui->graphicsView->setAlignment(Qt::AlignCenter);
    ui->graphicsView->setScene(new QGraphicsScene(this));
    connect(ui->symbolWidget, &SymbolWidget::valueChanged, [&] (int value)
    {
        ui->graphicsView->scene()->clear();
        int row = ui->symbolTable->selectionModel()->currentIndex().row();
        viewSymbol(row, value);
    });


    QStandardItemModel *listModel = new QStandardItemModel(ui->listCharacters);
    ui->listCharacters->setModel(listModel);

    const QListView *pListView = ui->listCharacters;
    connect(listModel, &QStandardItemModel::itemChanged, [pListView, listModel]
            (QStandardItem * item)
    {
        const QModelIndex index = listModel->indexFromItem(item);
        QItemSelectionModel *selModel = pListView->selectionModel();
        selModel->select(QItemSelection(index, index),
                         item->checkState () == Qt::Checked ?
                         QItemSelectionModel::Select :
                         QItemSelectionModel::Deselect);
    });
    connect (ui->listCharacters->selectionModel(),
             &QItemSelectionModel::selectionChanged,
             [listModel](const QItemSelection & selected, const QItemSelection & deselected)
    {
        for (const QModelIndex &index : selected.indexes())
            listModel->itemFromIndex(index)->setCheckState(Qt::Checked);
        for (const QModelIndex &index : deselected.indexes())
            listModel->itemFromIndex(index)->setCheckState(Qt::Unchecked);
    });
    prepareCodeNumbersList(listModel);

    disableAction(true);
    setStyleStrategy(QFont::NoAntialias);
}
MainWindow::~MainWindow ()
{
    ui->symbolTable->clearSelection();
    delete ui;
}

void MainWindow::createSelectionModel() const
{
    QAbstractItemModel *model = ui->symbolTable->model();
    QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
    ui->symbolTable->setSelectionModel(selectionModel);
    connect(selectionModel, &QItemSelectionModel::selectionChanged, this,
            &MainWindow::tableSelection);
}

void MainWindow::prepareCodeNumbersList(QStandardItemModel *listModel)
{
    listModel->setItem(CodeNumbers::LatinLetters,
                       createCodeListItem("[A-z]"));
    listModel->setItem(CodeNumbers::CyrillicLetters,
                       createCodeListItem("[А-я]"));
    listModel->setItem(CodeNumbers::Numbers,
                       createCodeListItem("[0-9]"));
    listModel->setItem(CodeNumbers::LatinSupplement,
                       createCodeListItem(m_codeNumbers[CodeNumbers::LatinSupplement]));
    listModel->setItem(CodeNumbers::Arrows,
                       createCodeListItem("Arrows"));
    listModel->setItem(CodeNumbers::BoxDrawing,
                       createCodeListItem("Box drawing", Qt::Unchecked));
    listModel->setItem(CodeNumbers::BlockElements,
                       createCodeListItem("Block elements", Qt::Unchecked));
    listModel->setItem(CodeNumbers::GeometricShapes,
                       createCodeListItem("Geometric shapes", Qt::Unchecked));
    listModel->setItem(CodeNumbers::MiscellanepousSymbols,
                       createCodeListItem("Miscellanepous symbols", Qt::Unchecked));
    //
    listModel->setItem(CodeNumbers::UnknownSymbol,
                       createCodeListItem("Unknown symbol", Qt::Checked));
}

QStandardItem *MainWindow::createCodeListItem(QString txt, Qt::CheckState state)
{
    QStandardItem *listItem = new QStandardItem(txt);
    listItem->setCheckable(true);
    listItem->setData(Qt::Unchecked, Qt::CheckStateRole);
    listItem->setCheckState(state);
    return listItem;
}

QPair<int, int> MainWindow::viewSymbol(int row, int zoom)
{
    QPair<int, int> size(0, 0);

    QPixmap pixmap = ui->symbolTable->model()->charPixmap(row);
    if (pixmap.isNull())
        return size;

    QImage::Format format = QImage::Format_Grayscale8;
    if (QFont::NoAntialias & m_font.styleStrategy())
        format = QImage::Format_Mono;
    pixmap = QPixmap::fromImage(pixmap.toImage().convertToFormat(format,
                                                                 Qt::AvoidDither));

    size.first = pixmap.width();
    int width = size.first * zoom;
    size.second = pixmap.height();
    int height = size.second * zoom;
    ui->graphicsView->scene()->setSceneRect(0, 0, width, height);
    pixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio);

    if (zoom > 3)
    {
        QPainter painter(&pixmap);
        painter.setPen(Qt::gray);

        for (int n = zoom; n < width; n += zoom)
            painter.drawLine (n, 0, n, height);
        for (int n = zoom; n < height; n += zoom)
            painter.drawLine (0, n, width, n);
    }
    ui->graphicsView->scene()->addPixmap(pixmap);

    return size;
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
        ui->symbolWidget->drawSize();
        return;
    }

    QPair<int, int> size = viewSymbol(row, ui->symbolWidget->zoom());
    ui->symbolWidget->drawSize(size.first, size.second);
}

void MainWindow::on_updateTbl_clicked()
{
    prepareTable(m_font);
}

void MainWindow::prepareTable(QFont &font)
{
    QModelIndexList selections =
        ui->listCharacters->selectionModel()->selectedRows();
    if (selections.size() == 0)
    {
        ui->symbolTable->model()->clear();
        ui->symbolWidget->drawSize();
        disableAction(true);
        return;
    }

    int selRow = -1;
    QItemSelectionModel *selModel = ui->symbolTable->selectionModel();
    if (selModel->hasSelection())
    {
        selRow = selModel->selectedRows()[0].row();
        selModel->clearSelection();
    }

    ui->graphicsView->scene()->clear();

    sbFontStyleName->setText(font.styleName());
    sbFontName->setText(font.family());
    sbFontSize->setText(QString ("%1").arg(font.pointSize()));

    QFontMetrics fm(font);
    int height = fm.height();

    int rowCount = 0;
    foreach (QModelIndex sel, selections)
    {
        uint8_t row = sel.row();
        ui->symbolTable->setRowCount(rowCount + m_codeNumbers[row].size());
        for (const QChar *c = m_codeNumbers[row].constData(); !c->isNull(); ++c)
        {
            int width = fm.width(*c);

            QPixmap pixmap(width, height);
            QPainter painter(&pixmap);
            painter.setFont(font);
            painter.fillRect(0, 0, width, height, Qt::white);
            if (*c == m_codeNumbers[CodeNumbers::UnknownSymbol].at(0))
                drawUnknowSymbol(painter, height, width);
            else
                painter.drawStaticText(0, 0, QStaticText(*c));

            ui->symbolTable->setItem(rowCount++, new SymbolTableItem(c->unicode(), pixmap));
        }
    }
    ui->symbolTable->resizeColumnsToContents();
    ui->symbolTable->resizeRowsToContents();

    emit ui->symbolTable->selectRow(selRow);
}

void MainWindow::drawUnknowSymbol(QPainter &painter, int height, int width)
{
    int c = (height - width) / 2;
    painter.fillRect(0, c, width, width, Qt::black);
    painter.setPen(Qt::white);
    painter.drawLine(1, c + 1, width - 1 - 1, width - 1 + c - 1);
    painter.drawLine(1, width - 1 + c - 1, width - 1 - 1, c + 1);
}

void MainWindow::createToolBar()
{
    ui->mainToolBar->addAction(ui->actionImportFont);
    ui->mainToolBar->addAction(ui->actionExportFontC);
}

void MainWindow::createStatusBar()
{
    QLabel *lb = new QLabel("Font name: ", statusBar());
    lb->setAlignment(Qt::AlignLeading);
    statusBar()->addWidget(lb);
    sbFontName = new QLabel(statusBar());
    statusBar()->addWidget(sbFontName);

    statusBar()->addWidget(separator());

    lb = new QLabel("Font size: ", statusBar());
    lb->setAlignment(Qt::AlignLeading);
    statusBar()->addWidget(lb);
    sbFontSize = new QLabel(statusBar());
    statusBar()->addWidget(sbFontSize);

    statusBar()->addWidget(separator());

    statusBar()->addWidget(new QLabel("Font style:", statusBar()));

    sbFontStyleName = new QLabel(statusBar());
    sbFontStyleName->setAlignment(Qt::AlignLeading);
    statusBar()->addWidget(sbFontStyleName);

    statusBar()->addWidget(separator());

    sbFontStyleStrategy = new QLabel("Antialias", statusBar());
    sbFontStyleStrategy->setAlignment(Qt::AlignLeading);
    sbFontStyleStrategy->setVisible(false);
    statusBar()->addWidget(sbFontStyleStrategy);
}

void MainWindow::setStyleStrategy(QFont::StyleStrategy styleStrategy)
{
    m_font.setStyleStrategy((QFont::StyleStrategy) (m_font.styleStrategy() ^
                                                    styleStrategy));
}

void MainWindow::disableAction(bool disable)
{
    ui->symbolTable->clearSelection();
    ui->graphicsView->scene()->clear();

    ui->actionExportFontC->setEnabled(!disable);
    ui->actionNoAntialias->setEnabled(!disable);
}

void MainWindow::on_actionImportFont_triggered()
{
    QFont::StyleStrategy styleStrategy = m_font.styleStrategy();
    bool ok;
    m_font = QFontDialog::getFont(&ok, m_font, this);
    m_font.setStyleStrategy(styleStrategy);
    if (!ok)
        return;

    prepareTable(m_font);
    disableAction(false);
}

void MainWindow::on_actionExportFontC_triggered()
{
    FontInfo fontInfo
    {
        m_font.styleStrategy() &QFont::NoAntialias ? FontMode::Bitmap : FontMode::Antialias,
        m_font.family(), m_font.pointSize(), m_font.styleName()
    };

    ExportDialog exp(fontInfo,  ui->symbolTable->items(), this);
    exp.exec();
}

void MainWindow::on_actionNoAntialias_triggered(bool e)
{
    sbFontStyleStrategy->setVisible(!e);
    setStyleStrategy(QFont::NoAntialias);
    prepareTable(m_font);
}

void MainWindow::on_actionExit_triggered()
{
    close();
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
