#ifndef LISTCHARACTERSDIALOG_H
#define LISTCHARACTERSDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include <unordered_map>

namespace Ui
{
class ListCharactersDialog;
}

class ListCharactersDialog final : public QDialog
{
        Q_OBJECT

        struct CodeNumbers final : std::unordered_map<quint8, QPair<quint16, quint16>>
        {
            CodeNumbers();

            enum eCode
            {
                // Latin script
                BasicLatin_Latin1Supplement = 0,
                LatinExtended_A,
                LatinExtended_B,
                LatinExtendedAdditional,
                // Phonetic scripts
                IPAExtensions,
                SpacingModifierLetters,
                // Greek and Coptic
                GreekCoptic,
                GreekExtended,
                //
                Cyrillic,
                //
                UnicodeSymbols,
                //
                GeneralPunctuation,
                //
                SuperscriptsSubscripts,
                //
                CurrencySymbols,
                //
                LetterlikeSymbols,
                //
                NumberForms,
                //
                Arrows,
                //
                MathematicalOperators,
                //
                MiscellaneousTechnical,
                //
                EnclosedAlphanumerics,
                //
                BoxDrawing,
                //
                BlockElements,
                //
                GeometricShapes,
                //
                MiscellaneousSymbols,
                //
                AlphabeticPresentationForms,
                //                // Game symbols
                //                MahjongTiles,
                //                DominoTiles,
                //                PlayingCards,
                //
                Specials
            };
        };

    public:
        explicit ListCharactersDialog(const QFont &font, QWidget *pParent);
        virtual ~ListCharactersDialog();

    protected:
        void changeEvent(QEvent *e);

    private slots:
        void on_addSelected_clicked();

    private:
        void prepareList();
        void prepareCodeNumbersList(QStandardItemModel *pListModel);

        void resizeEvent(QResizeEvent *event);

    signals:
        void selectedChars(const QVector<QChar> chars);

    private:
        Ui::ListCharactersDialog *ui;

        CodeNumbers m_codeNumbers;
};

#endif // LISTCHARACTERSDIALOG_H
