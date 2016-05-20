#ifndef FIXEDCHARACTERSTAB_H
#define FIXEDCHARACTERSTAB_H

#include "icharacters.h"

#include <QWidget>
#include <QItemSelection>
#include <QStandardItem>

#include <unordered_map>

namespace Ui
{
class FixedCharactersPage;
}

class FixedCharactersPage final : public QWidget, public ICharacters
{
        Q_OBJECT
        Q_INTERFACES(ICharacters)

        struct CodeNumbers final : std::unordered_map<quint8, QString>
        {
            CodeNumbers();

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

    public:
        explicit FixedCharactersPage(QWidget *pParent = nullptr);
        virtual ~FixedCharactersPage();

    public:
        const QString getCharacters() override;

    protected:
        void changeEvent(QEvent *e);

    public:
        static QChar UnknownSymbol;

    private:
        void prepareListCharacters();
        void prepareCodeNumbersList(QStandardItemModel *pListModel);
        QStandardItem *createCodeListItem(QString txt, Qt::CheckState state = Qt::Checked);

    private:
        Ui::FixedCharactersPage *ui;

        CodeNumbers m_codeNumbers;
};

#endif // FIXEDCHARACTERSTAB_H
