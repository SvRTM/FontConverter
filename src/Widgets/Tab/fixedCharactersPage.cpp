#include "fixedCharactersPage.h"
#include "ui_fixedCharactersPage.h"


QChar FixedCharactersPage::UnknownSymbol = QChar(0xFFFF);

FixedCharactersPage::CodeNumbers::CodeNumbers()
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
    operator[](UnknownSymbol) = FixedCharactersPage::UnknownSymbol;
}

FixedCharactersPage::FixedCharactersPage(QWidget *pParent)
    : QWidget(pParent)
    , ui(new Ui::FixedCharactersPage)
{
    ui->setupUi(this);

    prepareListCharacters();
}
FixedCharactersPage::~FixedCharactersPage()
{
    delete ui;
}

void FixedCharactersPage::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

const QString FixedCharactersPage::getCharacters()
{
    QModelIndexList selections = ui->listCharactersView->selectionModel()->selectedRows();
    if (selections.size() == 0)
        return nullptr;

    QByteArray ba;
    foreach (const QModelIndex sel, selections)
    {
        ba.append(m_codeNumbers[sel.row()]);
    }
    return ba;
}

void FixedCharactersPage::prepareListCharacters()
{
    QListView *pListView = ui->listCharactersView;

    QStandardItemModel *pListModel = new QStandardItemModel(ui->listCharactersView);
    pListView->setModel(pListModel);

    connect(pListModel, &QStandardItemModel::itemChanged,
            [pListView, pListModel] (QStandardItem * item)
    {
        const QModelIndex index = pListModel->indexFromItem(item);
        QItemSelectionModel *pSelModel = pListView->selectionModel();
        pSelModel->select(QItemSelection(index, index),
                          item->checkState() == Qt::Checked
                          ? QItemSelectionModel::Select
                          : QItemSelectionModel::Deselect);
    });
    connect(pListView->selectionModel(), &QItemSelectionModel::selectionChanged,
            [pListModel] (const QItemSelection & selected, const QItemSelection & deselected)
    {
        for (const QModelIndex &index : selected.indexes())
            pListModel->itemFromIndex(index)->setCheckState(Qt::Checked);
        for (const QModelIndex &index : deselected.indexes())
            pListModel->itemFromIndex(index)->setCheckState(Qt::Unchecked);
    });
    prepareCodeNumbersList(pListModel);
}
void FixedCharactersPage::prepareCodeNumbersList(QStandardItemModel *pListModel)
{
    pListModel->setItem(CodeNumbers::LatinLetters, createCodeListItem("[A-z]"));
    pListModel->setItem(CodeNumbers::CyrillicLetters, createCodeListItem("[А-я]"));
    pListModel->setItem(CodeNumbers::Numbers, createCodeListItem("[0-9]"));
    pListModel->setItem(CodeNumbers::LatinSupplement,
                        createCodeListItem(m_codeNumbers[CodeNumbers::LatinSupplement]));
    pListModel->setItem(CodeNumbers::Arrows, createCodeListItem("Arrows"));
    pListModel->setItem(CodeNumbers::BoxDrawing, createCodeListItem("Box drawing",
                        Qt::Unchecked));
    pListModel->setItem(CodeNumbers::BlockElements, createCodeListItem("Block elements",
                        Qt::Unchecked));
    pListModel->setItem(CodeNumbers::GeometricShapes, createCodeListItem("Geometric shapes",
                        Qt::Unchecked));
    pListModel->setItem(CodeNumbers::MiscellanepousSymbols,
                        createCodeListItem("Miscellanepous symbols", Qt::Unchecked));
    //
    pListModel->setItem(CodeNumbers::UnknownSymbol, createCodeListItem("Unknown symbol",
                        Qt::Checked));
}
QStandardItem *FixedCharactersPage::createCodeListItem(QString txt, Qt::CheckState state)
{
    QStandardItem *pItem = new QStandardItem(txt);
    pItem->setCheckable(true);
    pItem->setData(Qt::Unchecked, Qt::CheckStateRole);
    pItem->setCheckState(state);
    return pItem;
}
