#ifndef SELECTEDCHARACTERSTAB_H
#define SELECTEDCHARACTERSTAB_H

#include "icharacters.h"
#include <QWidget>

namespace Ui
{
class SelectedCharactersPage;
}

class SelectedCharactersPage final : public QWidget, public ICharacters
{
        Q_OBJECT
        Q_INTERFACES(ICharacters)

    public:
        explicit SelectedCharactersPage(QWidget *pParent = 0);
        virtual ~SelectedCharactersPage();

    public:
        void setFont(const QFont &font);
        const QString getCharacters() override;

    protected:
        void changeEvent(QEvent *e);

    private slots:
        void on_listCharactersBtn_clicked();
        void on_enteredCharacters_textChanged();
        void on_clearBtn_clicked();


    private:
        Ui::SelectedCharactersPage *ui;
        const QFont *font = nullptr;
};

#endif // SELECTEDCHARACTERSTAB_H
