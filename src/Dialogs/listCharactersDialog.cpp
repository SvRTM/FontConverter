/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

#include "listCharactersDialog.h"
#include "ui_listCharactersDialog.h"

#include "Widgets/listCharactersScene.h"

ListCharactersDialog::CodeNumbers::CodeNumbers()
{
    // Latin script
    operator[](BasicLatin_Latin1Supplement) = qMakePair(0x0000, 0x00FF);
    operator[](LatinExtended_A) = qMakePair(0x0100, 0x017F);
    operator[](LatinExtended_B) = qMakePair(0x0180, 0x024F);
    operator[](LatinExtendedAdditional) = qMakePair(0x1E02, 0x1EF3);
    // Phonetic scripts
    operator[](IPAExtensions) = qMakePair(0x0259, 0x0292);
    operator[](SpacingModifierLetters) = qMakePair(0x02B0, 0x02FF);
    // Greek and Coptic
    operator[](GreekCoptic) = qMakePair(0x0374, 0x03FF);
    operator[](GreekExtended) = qMakePair(0x1F00, 0x1FFF);
    //
    operator[](Cyrillic) = qMakePair(0x0400, 0x04FF);
    //
    operator[](UnicodeSymbols) = qMakePair(0x2013, 0x204A);
    //
    operator[](GeneralPunctuation) = qMakePair(0x2000, 0x206F);
    //
    operator[](SuperscriptsSubscripts) = qMakePair(0x2070, 0x209F);
    //
    operator[](CurrencySymbols) = qMakePair(0x20A0, 0x20CF);
    //
    operator[](LetterlikeSymbols) = qMakePair(0x2100, 0x214F);
    //
    operator[](NumberForms) = qMakePair(0x2150, 0x218F);
    //
    operator[](Arrows) = qMakePair(0x2190, 0x21FF);
    //
    operator[](MathematicalOperators) = qMakePair(0x2200, 0x22FF);
    //
    operator[](MiscellaneousTechnical) = qMakePair(0x2300, 0x23FF);
    //
    operator[](EnclosedAlphanumerics) = qMakePair(0x2460, 0x24FF);
    //
    operator[](BoxDrawing) = qMakePair(0x2500, 0x257F);
    //
    operator[](BlockElements) = qMakePair(0x2580, 0x259F);
    //
    operator[](GeometricShapes) = qMakePair(0x25A0, 0x25FF);
    //
    operator[](MiscellaneousSymbols) = qMakePair(0x2600, 0x26FF);
    //
    operator[](AlphabeticPresentationForms) = qMakePair(0xFB00, 0xFB4F);
    //    // Game symbols
    //    operator[](MahjongTiles) = qMakePair(0x1F000, 0x1F02F);
    //    operator[](DominoTiles) = qMakePair(0x1F030, 0x1F09F);
    //    operator[](PlayingCards) = qMakePair(0x1F0A0, 0x1F0FF);
    //
    operator[](Specials) = qMakePair(0xFFF0, 0xFFFF);
}

ListCharactersDialog::ListCharactersDialog(const QFont &font, QWidget *pParent)
    : QDialog(pParent)
    , ui(new Ui::ListCharactersDialog)
{
    ui->setupUi(this);
    ui->listCharactersView->setScene(new ListCharactersScene(font, this));
    prepareList();
}
ListCharactersDialog::~ListCharactersDialog()
{
    delete ui;
}

void ListCharactersDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void ListCharactersDialog::prepareList()
{
    QListView *pListView = ui->listCharacters;

    QStandardItemModel *pListModel = new QStandardItemModel(pListView);
    pListView->setModel(pListModel);

    connect (pListView->selectionModel(), &QItemSelectionModel::selectionChanged,
             [ &, pListModel] (const QItemSelection & selected, const QItemSelection &)
    {
        QModelIndexList idxs = selected.indexes();
        if (idxs.size() == 0)
            return;

        QPair<quint16, quint16> range = m_codeNumbers[idxs[0].row()];
        ListCharactersScene *scene = qobject_cast<ListCharactersScene *>
                                     (ui->listCharactersView->scene());
        scene->createListCharacters(range.first, range.second);
    });

    prepareCodeNumbersList(pListModel);

    pListView->setCurrentIndex(pListModel->index(0, 0));
}
void ListCharactersDialog::prepareCodeNumbersList(QStandardItemModel *pListModel)
{
    // Latin script
    pListModel->setItem(CodeNumbers::BasicLatin_Latin1Supplement,
                        new QStandardItem("Basic Latin and Latin-1 Supplement"));
    pListModel->setItem(CodeNumbers::LatinExtended_A, new QStandardItem("Latin Extended-A"));
    pListModel->setItem(CodeNumbers::LatinExtended_B, new QStandardItem("Latin Extended-B"));
    pListModel->setItem(CodeNumbers::LatinExtendedAdditional,
                        new QStandardItem("Latin Extended Additional"));
    // Phonetic scripts
    pListModel->setItem(CodeNumbers::IPAExtensions, new QStandardItem("IPA Extensions"));
    pListModel->setItem(CodeNumbers::SpacingModifierLetters,
                        new QStandardItem("Spacing modifier letters"));
    // Greek and Coptic
    pListModel->setItem(CodeNumbers::GreekCoptic, new QStandardItem("Greek and Coptic"));
    pListModel->setItem(CodeNumbers::GreekExtended, new QStandardItem("Greek Extended"));
    //
    pListModel->setItem(CodeNumbers::Cyrillic, new QStandardItem("Cyrillic"));
    //
    pListModel->setItem(CodeNumbers::UnicodeSymbols, new QStandardItem("Unicode symbols"));
    //
    pListModel->setItem(CodeNumbers::GeneralPunctuation,
                        new QStandardItem("General Punctuation"));
    //
    pListModel->setItem(CodeNumbers::SuperscriptsSubscripts,
                        new QStandardItem("Superscripts and Subscripts"));
    //
    pListModel->setItem(CodeNumbers::CurrencySymbols, new QStandardItem("Currency Symbols"));
    //
    pListModel->setItem(CodeNumbers::LetterlikeSymbols,
                        new QStandardItem("Letterlike Symbols"));
    //
    pListModel->setItem(CodeNumbers::NumberForms, new QStandardItem("Number Forms"));
    //
    pListModel->setItem(CodeNumbers::Arrows, new QStandardItem("Arrows"));
    //
    pListModel->setItem(CodeNumbers::MathematicalOperators,
                        new QStandardItem("Mathematical Operators"));
    //
    pListModel->setItem(CodeNumbers::MiscellaneousTechnical,
                        new QStandardItem("Miscellaneous Technical"));
    //
    pListModel->setItem(CodeNumbers::EnclosedAlphanumerics,
                        new QStandardItem("Enclosed Alphanumerics"));
    //
    pListModel->setItem(CodeNumbers::BoxDrawing, new QStandardItem("Box Drawing"));
    //
    pListModel->setItem(CodeNumbers::BlockElements, new QStandardItem("Block Elements"));
    //
    pListModel->setItem(CodeNumbers::GeometricShapes, new QStandardItem("Geometric Shapes"));
    //
    pListModel->setItem(CodeNumbers::MiscellaneousSymbols,
                        new QStandardItem("Miscellaneous Symbols"));
    //
    pListModel->setItem(CodeNumbers::AlphabeticPresentationForms,
                        new QStandardItem("Alphabetic Presentation Forms"));
    //    // Game symbols
    //    listModel->setItem(CodeNumbers::MahjongTiles, new QStandardItem("Mahjong Tiles"));
    //    listModel->setItem(CodeNumbers::DominoTiles, new QStandardItem("Domino Tiles"));
    //    listModel->setItem(CodeNumbers::PlayingCards, new QStandardItem("Playing cards"));
    //
    pListModel->setItem(CodeNumbers::Specials, new QStandardItem("Specials"));
}

void ListCharactersDialog::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    QSize size = ui->listCharactersView->geometry().size();
    ui->listCharactersView->scene()->setSceneRect(0, 0, size.width(), size.height());
}

void ListCharactersDialog::on_addSelected_clicked()
{
    ListCharactersScene *pScene = qobject_cast<ListCharactersScene *>
                                  (ui->listCharactersView->scene());
    emit selectedChars(pScene->getSelected());
}
