#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDialog>
#include <QGraphicsEllipseItem>

#include "tablemodel.h"
#include <QtDebug>
#include <QStaticText>
#include <QTextCodec>

#include <QtDebug>

MainWindow::MainWindow (QWidget* parent) : QMainWindow (parent), ui (new Ui::MainWindow)
{
    ui->setupUi (this);

    createActions ();
    createMenuBar ();
    createToolBar ();
    createStatusBar ();

    QGraphicsScene* scene = new QGraphicsScene ();
    ui->graphicsView->setScene (scene);
    textItem =
    scene->addSimpleText ("Широкая электрификация южных губерний даст мощный толчок подъёму "
                          "сельского хозяйства.\nJackdaws love my big sphinx of quartz.");
    ui->graphicsView->show ();

    QGraphicsScene* scene2 = new QGraphicsScene ();
    ui->graphicsView_2->setScene (scene2);
    ui->graphicsView_2->show ();
    ui->graphicsView_2->scale (20, 20);

    connect (ui->graphicsView, &GraphicsView::smouseMoveEvent, this, &MainWindow::_mouseMoveEvent);
    connect (ui->pushButton, &QPushButton::pressed, this, &MainWindow::released);

    TableModel* model = new TableModel (this);
    ui->tableView->setModel (model);
    QItemSelectionModel* selectionModel = new QItemSelectionModel (model);
    ui->tableView->setSelectionModel (selectionModel);

    ui->tableView->setRowCount (256);


    connect (selectionModel, &QItemSelectionModel::currentChanged, this, &MainWindow::currentRowChanged);
    connect (selectionModel, &QItemSelectionModel::selectionChanged, this, &MainWindow::selectionChanged);

    QGraphicsScene* scene3 = new QGraphicsScene ();
    ui->graphicsView_3->setScene (scene3);

    createChBoxStyleStrategy ();
}
void MainWindow::selectionChanged (const QItemSelection& selected, const QItemSelection& deselected)
{
    qDebug () << "selCha";

    ui->graphicsView_3->scene ()->clear ();
    //qDebug()<<"rowCh "<< current.row ()<<"\n";
    QModelIndexList ni = selected.indexes ();
    int row = 0;
    if (ni.size () != 0)
        row = ni[0].row ();
    else
        row = deselected.indexes ()[0].row ();

    QPixmap mp = (qobject_cast<TableModel*> (ui->tableView->model ()))->getData (row);
    selectedPixmap = mp;
    if (mp.isNull ())
        return;
    QPixmap m =
    QPixmap::fromImage (mp.toImage ().convertToFormat (QImage::Format_Grayscale8, Qt::AvoidDither));

    int width = m.width ();
    int height = m.height ();
    QPixmap pm1 = m.scaled (width * 10, height * 10, Qt::KeepAspectRatio);
    QPainter pa1 (&pm1);
    pa1.setPen (Qt::gray);
    for (int n = 0; n <= width * 10; n += 10)
        pa1.drawLine (n, 0, n, height * 10);
    for (int n = 0; n <= height * 10; n += 10)
        pa1.drawLine (0, n, width * 10, n);
    ui->graphicsView_3->scene ()->addPixmap (pm1);
}

MainWindow::~MainWindow ()
{
    delete ui;
}
void MainWindow::currentRowChanged (const QModelIndex& current, const QModelIndex&)
{
    //    ui->graphicsView_3->scene ()->clear ();
    //qDebug()<<"rowCh "<< current.row ()<<"\n";
    //    QPixmap mp = (qobject_cast<TableModel*> (ui->tableView->model ()))->getData (current.row ());
    //    selectedPixmap = mp;
    //    if (mp.isNull ())
    //        return;
    //    QPixmap m =
    //    QPixmap::fromImage (mp.toImage ().convertToFormat (QImage::Format_Grayscale8, Qt::AvoidDither));

    //    int width = m.width ();
    //    int height = m.height ();
    //    QPixmap pm1 = m.scaled (width * 10, height * 10, Qt::KeepAspectRatio);
    //    QPainter pa1 (&pm1);
    //    pa1.setPen (Qt::gray);
    //    for (int n = 0; n <= width * 10; n += 10)
    //        pa1.drawLine (n, 0, n, height * 10);
    //    for (int n = 0; n <= height * 10; n += 10)
    //        pa1.drawLine (0, n, width * 10, n);
    //    ui->graphicsView_3->scene ()->addPixmap (pm1);
}


void MainWindow::_mouseMoveEvent (QMouseEvent* event)
{
    QPixmap pixmap (10, 10);
    QPainter painter (&pixmap);
    painter.fillRect (0, 0, 10, 10, Qt::white);
    ui->graphicsView->render (&painter, QRectF (0, 0, 10, 10), QRect (event->pos (), QSize (10, 10)));
    //QGraphicsItem *item =    graphicsView->itemAt( event->pos());
    // pa->drawText(0,0,"dsfs");
    QImage im = pixmap.toImage (); //.convertToFormat (QImage::Format_Grayscale8, Qt::AvoidDither);

    //ui->graphicsView_2->scene()->addPixmap( pixmap);
    ui->graphicsView_2->scene ()->addPixmap (QPixmap::fromImage (im));
}

void MainWindow::released ()
{
    bool ok;
    _font = QFontDialog::getFont (&ok, _font, this);
    if (ok)
        prepareTable (_font);
}

void MainWindow::prepareTable (QFont& font)
{
    qDebug () << "prepare";
    QItemSelectionModel* select = ui->tableView->selectionModel ();
    QModelIndex in; //= select->selectedRows ()[0];
    int rr = -1;
    if (select->hasSelection ())
    {
        /*   QModelIndex */ in = select->selectedRows ()[0];
        rr = in.row ();
        qDebug () << rr << " " << select->selection () << "\n";
        //emit select->select(select->selection(), QItemSelectionModel::ToggleCurrent);
        //emit ui->tableView->selectRow (rr+1);
        //QModelIndex in1 = ui->tableView->model()->index(0, 0, QModelIndex());
        //QModelIndex in2 = ui->tableView->model()->index(ui->tableView->model()->rowCount() ,0, QModelIndex());
        //ui->tableView->selectAll();//model()-> updateViews( in1, in2 );
        ui->graphicsView_3->scene ()->clear ();
    }
    //ui->tableView->selectAll ();
    /*    qDebug () << "Font: " << font.family () << " weigth:" << font.weight ()
              << "   Point size:" << font.pointSize () << "  stretch:" << font.stretch ()
              << "  wordSpacing:" << font.wordSpacing ();*/

    QFontMetrics fm (font);
    //qDebug () << "height:" << fm.height () << "  width:" << fm.width ("H");

    textItem->setFont (font);
    fontName->setText (font.family ());
    fontSize->setText (QString ("%1").arg (font.pointSize ()));

    QTextCodec* codec = QTextCodec::codecForName ("Windows-1251");
    int maxWidth = 0;
    for (uchar i = 0; i < 255; i++)
    {
        char ascii[1] = { (char)i };
        QChar ch = codec->toUnicode (ascii)[0];
        int width = fm.width (ch);
        int height = fm.height ();
        maxWidth = qMax (maxWidth, width);

        QPixmap pm (width, height);
        QPainter pa (&pm);
        pa.setFont (font);
        pa.fillRect (0, 0, width, height, Qt::white);
        pa.drawStaticText (0, 0, QStaticText (codec->toUnicode (ascii)));
        /*
        QPixmap pm1 = pm.scaled (width * 10, height * 10, Qt::KeepAspectRatio);
        QPainter pa1 (&pm1);
        // pa1.setBrush(Qt::CrossPattern);
        //pa1.drawRect(0, 0, width*10, height*10);
        pa1.setPen (Qt::gray);
        for (int n = 0; n <= width * 10; n += 10)
            pa1.drawLine (n, 0, n, height * 10);
        for (int n = 0; n <= height * 10; n += 10)
            pa1.drawLine (0, n, width * 10, n);
        ui->tableView->setRowHeight (i, height * 10 + 10);*/
        ui->tableView->setItem (i, new TableItem (i, pm));
    }

    //ui->tableView->setColumnWidth (2, maxWidth * 10 + 10);
    ui->tableView->resizeColumnsToContents ();
    ui->tableView->resizeRowsToContents ();

    ui->plainTextEdit->setFont (font);
    emit select->select (select->selection (), QItemSelectionModel::Deselect);
    if (rr != -1)
    {
        // select->select(in, QItemSelectionModel::Rows);
        emit ui->tableView->selectRow (rr);
        //  qDebug () << "selected " << rr;
        emit select->select (select->selection (), QItemSelectionModel::Select);
    }
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
    QLabel* label = new QLabel ("Font name: ");
    label->setAlignment (Qt::AlignLeading);
    statusBar ()->addWidget (label);
    fontName = new QLabel ();
    statusBar ()->addWidget (fontName);

    QFrame* line = new QFrame (this);
    line->setFrameShape (QFrame::VLine);
    line->setFrameShadow (QFrame::Sunken);
    statusBar ()->addWidget (line);

    label = new QLabel ("Font size: ");
    label->setAlignment (Qt::AlignLeading);
    statusBar ()->addWidget (label);
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
    qDebug ("onImport()");

    QImage image = selectedPixmap.toImage ().convertToFormat (QImage::Format_Grayscale8, Qt::AvoidDither);
    int height = image.height ();
    int width = image.width ();

    for (int i = 0; i < height; i++)
    {
        QString k;
        const uchar* n = image.constScanLine (i);
        for (int x = 0; x < width; x++)
        {
            QRgb px = image.pixel (x, i);

            //          if (n[x]==255)
            //if (qGray(px)==255)
            if (qGray (px) >= 178)
                k.append ("  ");
            else
                k.append ('#');

            //            k.append(QString("%1 ").arg(qGray(px)));
            // qDebug()<<n[x];
        }
        // k.append("\r\n");
        qDebug () << k;
    }
    // qDebug()<<k;
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
    _font.setStyleStrategy ((QFont::StyleStrategy) (_font.styleStrategy () ^ styleStrategy));
    prepareTable (_font);
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
