#include "selectedCharactersPage.h"
#include "ui_selectedCharactersPage.h"

#include "Dialogs/listCharactersDialog.h"
#include <QStringList>

SelectedCharactersPage::SelectedCharactersPage(QWidget *pParent) :
    QWidget(pParent),
    ui(new Ui::SelectedCharactersPage)
{
    ui->setupUi(this);
}
SelectedCharactersPage::~SelectedCharactersPage()
{
    delete ui;
}

void SelectedCharactersPage::changeEvent(QEvent *e)
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

void SelectedCharactersPage::setFont(const QFont &font)
{
    this->font = &font;
    QFont ft(font);
    ft.setPointSize(10);
    ui->enteredCharacters->setFont(ft);
    ui->usedCharacters->setFont(ft);
}
const QString SelectedCharactersPage::getCharacters()
{
    return ui->usedCharacters->toPlainText();
}

void SelectedCharactersPage::on_listCharactersBtn_clicked()
{
    ListCharactersDialog dlg(*font, this);
    connect(&dlg, &ListCharactersDialog::selectedChars, [&] (const QVector<QChar> chars)
    {
        QString txt = ui->enteredCharacters->toPlainText().append(QString(chars.constData(),
                      chars.size()));
        ui->enteredCharacters->setPlainText(txt);
    });
    dlg.exec();
}

void SelectedCharactersPage::on_enteredCharacters_textChanged()
{
    QString txt = ui->enteredCharacters->toPlainText();
    txt.remove("\n");
    QStringList strs = txt.split("");
    strs.removeDuplicates();
    const QString uniqueCharacters = strs.join("");
    ui->usedCharacters->setPlainText(uniqueCharacters);
}

void SelectedCharactersPage::on_clearBtn_clicked()
{
    ui->enteredCharacters->clear();
}
