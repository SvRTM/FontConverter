#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "tablemodel.h"

#include <QFontDialog>
#include <QStaticText>

#include <QtDebug>


CodeNumbers::CodeNumbers ()
{
    operator[](LatinLetters) = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";
    operator[](CyrillicLetters) = "АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
                                  "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
    operator[](Numbers) = "0123456789";
    operator[](LatinSupplement) = " <>()[]{}`~;:.,'\"\\|/?!@#$%^&*-+=_";
    operator[](Arrows) = "←↑→↓↔↕⇒⇐⇔⇑⇓↵";
    operator[](BoxDrawing) = "─│┌┐└┘├┤┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬";
    operator[](BlockElements) = "▀▄█▌▐░▒▓";
    operator[](GeometricShapes) = "■•◊○◄▼►▲▪▫";
    operator[](MiscellanepousSymbols) = "♠♣♥♦♀♂";
}


MainWindow::MainWindow (QWidget* parent) : QMainWindow (parent), ui (new Ui::MainWindow)
{
    ui->setupUi (this);

    createActions ();
    createMenuBar ();
    createToolBar ();
    createStatusBar ();

    createChBoxStyleStrategy ();


    connect (ui->pushButton, &QPushButton::pressed, this, &MainWindow::selectFont);


    QAbstractItemModel* model = ui->tableView->model ();
    QItemSelectionModel* selectionModel = new QItemSelectionModel (model);
    ui->tableView->setSelectionModel (selectionModel);
    connect (selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::tableSelection);


    ui->graphicsView_3->setScene (new QGraphicsScene (this));


    // Create list view item model
    QStandardItemModel* listModel = new QStandardItemModel (ui->listView);
    ui->listView->setModel (listModel);


    const QListView* pListView = ui->listView;
    connect (listModel, &QStandardItemModel::itemChanged, [pListView, listModel](QStandardItem* item)
             {
                 const QModelIndex index = listModel->indexFromItem (item);
                 QItemSelectionModel* selModel = pListView->selectionModel ();
                 selModel->select (QItemSelection (index, index), item->checkState () == Qt::Checked ?
                                                                  QItemSelectionModel::Select :
                                                                  QItemSelectionModel::Deselect);
             });
    connect (ui->listView->selectionModel (), &QItemSelectionModel::selectionChanged,
             [listModel](const QItemSelection& selected, const QItemSelection& deselected)
             {
                 for (const QModelIndex& index : selected.indexes ())
                     listModel->itemFromIndex (index)->setCheckState (Qt::Checked);
                 for (const QModelIndex& index : deselected.indexes ())
                     listModel->itemFromIndex (index)->setCheckState (Qt::Unchecked);
             });
    prepareCodeNumbersList (listModel);
}
MainWindow::~MainWindow ()
{
    delete ui;
}

void MainWindow::prepareCodeNumbersList (QStandardItemModel* listModel)
{
    listModel->setItem (CodeNumbers::LatinLetters, createCodeListItem ("[A-z]"));
    listModel->setItem (CodeNumbers::CyrillicLetters, createCodeListItem ("[А-я]"));
    listModel->setItem (CodeNumbers::Numbers, createCodeListItem ("[0-9]"));
    listModel->setItem (CodeNumbers::LatinSupplement,
                        createCodeListItem (m_codeNumbers[CodeNumbers::LatinSupplement]));
    listModel->setItem (CodeNumbers::Arrows, createCodeListItem ("Arrows"));
    listModel->setItem (CodeNumbers::BoxDrawing, createCodeListItem ("Box drawing", Qt::Unchecked));
    listModel->setItem (CodeNumbers::BlockElements, createCodeListItem ("Block elements", Qt::Unchecked));
    listModel->setItem (CodeNumbers::GeometricShapes, createCodeListItem ("Geometric shapes", Qt::Unchecked));
    listModel->setItem (CodeNumbers::MiscellanepousSymbols,
                        createCodeListItem ("Miscellanepous symbols", Qt::Unchecked));
}

QStandardItem* MainWindow::createCodeListItem (QString text, Qt::CheckState state)
{
    QStandardItem* listItem = new QStandardItem (text);
    listItem->setCheckable (true);
    listItem->setData (Qt::Unchecked, Qt::CheckStateRole);
    listItem->setCheckState (state);
    return listItem;
}

void MainWindow::tableSelection (const QItemSelection& selected, const QItemSelection& deselected)
{
    ui->graphicsView_3->scene ()->clear ();

    int row;
    if (!selected.isEmpty ())
        row = selected.indexes ()[0].row ();
    else if (!deselected.isEmpty ())
        row = deselected.indexes ()[0].row ();
    else
        return;

    QPixmap pixmap = ui->tableView->model ()->charPixmap (row);
    selectedPixmap = pixmap;
    if (pixmap.isNull ())
        return;
    pixmap = QPixmap::fromImage (pixmap.toImage ().convertToFormat (QImage::Format_Grayscale8, Qt::AvoidDither));

    int width = pixmap.width ();
    int height = pixmap.height ();
    pixmap = pixmap.scaled (width * 10, height * 10, Qt::KeepAspectRatio);
    QPainter painter (&pixmap);
    painter.setPen (Qt::gray);

    for (int n = 10; n < width * 10; n += 10)
        painter.drawLine (n, 0, n, height * 10);
    for (int n = 10; n < height * 10; n += 10)
        painter.drawLine (0, n, width * 10, n);

    ui->graphicsView_3->scene ()->addPixmap (pixmap);
}

void MainWindow::selectFont ()
{
    QFont::StyleStrategy styleStrategy = m_font.styleStrategy ();
    bool ok;
    m_font = QFontDialog::getFont (&ok, m_font, this);
    m_font.setStyleStrategy (styleStrategy);
    if (ok)
        prepareTable (m_font);
}

void MainWindow::prepareTable (QFont& font)
{
    QModelIndexList selections = ui->listView->selectionModel ()->selectedRows ();
    if (selections.size () == 0)
        return;

    fontName->setText (font.family ());
    fontSize->setText (QString ("%1").arg (font.pointSize ()));
    ui->plainTextEdit->setFont (font);

    QFontMetrics fm (font);
    int height = fm.height ();


    int rowCount = 0;
    foreach (QModelIndex sel, selections)
    {
        int row = sel.row ();
        ui->tableView->setRowCount (rowCount + m_codeNumbers[row].size ());
        for (const QChar* _char = m_codeNumbers[row].constData (); !_char->isNull (); ++_char)
        {
            int width = fm.width (*_char);

            QPixmap pixmap (width, height);
            QPainter painter (&pixmap);
            painter.setFont (font);
            painter.fillRect (0, 0, width, height, Qt::white);
            painter.drawStaticText (0, 0, QStaticText (*_char));
            ui->tableView->setItem (rowCount++, new TableItem (_char->unicode (), pixmap));
        }
    }
    ui->tableView->resizeColumnsToContents ();
    ui->tableView->resizeRowsToContents ();


    QItemSelectionModel* selModel = ui->tableView->selectionModel ();
    if (!selModel->hasSelection ())
        return;

    ui->graphicsView_3->scene ()->clear ();

    int row = selModel->selectedRows ()[0].row ();
    emit selModel->select (selModel->selection (), QItemSelectionModel::Deselect);
    emit ui->tableView->selectRow (row);
}


QAction* MainWindow::createAction (QString name, QString shortcut, QString tip)
{
    QAction* action = new QAction (name, this);
    action->setShortcut (shortcut);
    action->setStatusTip (tip);
    return action;
}

QAction* MainWindow::createAction (QString iconPath, QString name, QString shortcut, QString tip)
{
    QIcon icon (iconPath);
    QAction* action = new QAction (icon, name, this);
    action->setShortcut (shortcut);
    action->setStatusTip (tip);
    return action;
}

#define A_CONNECT(action, slot) \
    QObject::connect (action, &QAction::triggered, this, &MainWindow::slot)

void MainWindow::createActions ()
{
    importAction = createAction ("open.png", "&Import a font", "Ctrl+I", "Import a font");
    exitAction = createAction ("E&xit", "Ctrl+Q", "Exit program");

    A_CONNECT (importAction, onImport);
    A_CONNECT (exitAction, onExit);

    exportAction = createAction ("open.png", "&Expoprt", "Ctrl+E", "Export");
    A_CONNECT (exportAction, onExport);
}

void MainWindow::createMenuBar ()
{
    fileMenu = menuBar ()->addMenu ("&File");
    fileMenu->addAction (importAction);
    fileMenu->addSeparator ();
    fileMenu->addAction (exitAction);
}

void MainWindow::createToolBar ()
{
    fileBar = addToolBar ("file");
    fileBar->addAction (importAction);
    fileBar->addAction (exportAction);
}

void MainWindow::createStatusBar ()
{
    QLabel* lb = new QLabel ("Font name: ");
    lb->setAlignment (Qt::AlignLeading);
    statusBar ()->addWidget (lb);
    fontName = new QLabel ();
    statusBar ()->addWidget (fontName);

    QFrame* line = new QFrame (this);
    line->setFrameShape (QFrame::VLine);
    line->setFrameShadow (QFrame::Sunken);
    statusBar ()->addWidget (line);

    lb = new QLabel ("Font size: ");
    lb->setAlignment (Qt::AlignLeading);
    statusBar ()->addWidget (lb);
    fontSize = new QLabel ();
    statusBar ()->addWidget (fontSize);

    line = new QFrame (this);
    line->setFrameShape (QFrame::VLine);
    line->setFrameShadow (QFrame::Sunken);
    statusBar ()->addWidget (line);
}

void MainWindow::onImport ()
{
    qDebug ("onImport()");
}

void MainWindow::onExit ()
{
    qDebug ("onExit()");
}

void MainWindow::onExport ()
{
    QImage image = selectedPixmap.toImage ().convertToFormat (QImage::Format_Grayscale8, Qt::AvoidDither);
    int height = image.height ();
    int width = image.width ();

    for (int i = 0; i < height; i++)
    {
        QString k;
        for (int x = 0; x < width; x++)
        {
            QRgb px = image.pixel (x, i);
            if (qGray (px) >= 178)
                k.append ("  ");
            else
                k.append ('#');
        }
        qDebug () << k;
    }
}

#define CBOX_CONNECT(chbox, slot) \
    QObject::connect (chbox, &QCheckBox::stateChanged, this, &MainWindow::slot)
#define RBTN_CONNECT(chbox, slot) \
    QObject::connect (chbox, &QRadioButton::clicked, this, &MainWindow::slot)

void MainWindow::createChBoxStyleStrategy ()
{
    CBOX_CONNECT (ui->preferOutline, onPreferOutline);
    CBOX_CONNECT (ui->preferDevice, onPreferDevice);
    CBOX_CONNECT (ui->preferBitmap, onPreferBitmap);
    CBOX_CONNECT (ui->forceOutline, onForceOutline);
    CBOX_CONNECT (ui->preferDefault, onPreferDefault);
    CBOX_CONNECT (ui->preferAntialias, onPreferAntialias);
    CBOX_CONNECT (ui->noSubpixelAntialias, onNoSubpixelAntialias);
    CBOX_CONNECT (ui->noAntialias, onNoAntialias);
    CBOX_CONNECT (ui->openGLCompatible, onOpenGLCompatible);
    CBOX_CONNECT (ui->noFontMerging, onNoFontMerging);

    RBTN_CONNECT (ui->preferMatch, onPreferMatch);
    RBTN_CONNECT (ui->preferQuality, onPreferQuality);
    RBTN_CONNECT (ui->forceIntegerMetrics, onForceIntegerMetrics);
}
void MainWindow::setStyleStrategy (QFont::StyleStrategy styleStrategy)
{
    m_font.setStyleStrategy ((QFont::StyleStrategy) (m_font.styleStrategy () ^ styleStrategy));
    prepareTable (m_font);
}

void MainWindow::onPreferOutline (int)
{
    setStyleStrategy (QFont::PreferOutline);
}
void MainWindow::onPreferDevice (int)
{
    setStyleStrategy (QFont::PreferDevice);
}
void MainWindow::onPreferBitmap (int)
{
    setStyleStrategy (QFont::PreferBitmap);
}
void MainWindow::onForceOutline (int)
{
    setStyleStrategy (QFont::ForceOutline);
}
void MainWindow::onPreferDefault (int)
{
    setStyleStrategy (QFont::PreferDefault);
}
void MainWindow::onPreferAntialias (int)
{
    setStyleStrategy (QFont::PreferAntialias);
}
void MainWindow::onNoSubpixelAntialias (int)
{
    setStyleStrategy (QFont::NoSubpixelAntialias);
}
void MainWindow::onNoAntialias (int)
{
    setStyleStrategy (QFont::NoAntialias);
}
void MainWindow::onOpenGLCompatible (int)
{
    setStyleStrategy (QFont::OpenGLCompatible);
}
void MainWindow::onNoFontMerging (int)
{
    setStyleStrategy (QFont::NoFontMerging);
}

void MainWindow::onPreferMatch (bool)
{
    if (ui->preferMatch->isChecked ())
    {
        ui->preferQuality->setChecked (false);
        ui->forceIntegerMetrics->setChecked (false);
    }
    setStyleStrategy (QFont::PreferMatch);
}
void MainWindow::onPreferQuality (bool)
{
    if (ui->preferQuality->isChecked ())
    {

        ui->forceIntegerMetrics->setChecked (false);
        ui->preferMatch->setChecked (false);
    }
    setStyleStrategy (QFont::PreferQuality);
}
void MainWindow::onForceIntegerMetrics (bool)
{
    if (ui->forceIntegerMetrics->isChecked ())
    {
        ui->preferQuality->setChecked (false);
        ui->preferMatch->setChecked (false);
    }
    setStyleStrategy (QFont::ForceIntegerMetrics);
}


void MainWindow::changeEvent (QEvent* e)
{
    QMainWindow::changeEvent (e);
    switch (e->type ())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi (this);
        break;
    default:
        break;
    }
}
