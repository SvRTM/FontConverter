#include "export.h"
#include "fontexport.h"

#include "ui_export.h"


Export::Export(const int depth, const QList<TableItem *> &items, QWidget *parent)
    : QDialog(parent), ui(new Ui::Export)
{
    ui->setupUi (this);

    connect(ui->cancel,  &QPushButton::clicked, this,  &Export::reject);
    connect(ui->save, &QPushButton::clicked, this, &Export::save);

    IFontExport *exp;
    if (depth == 1)
        exp = new BitColor(items);
    else
        exp = new GrayscaleColor(items);
    QString str = exp->process();
    ui->plainTextEdit->setPlainText(str);

    delete exp;
}
Export::~Export()
{
    delete ui;
}

void Export::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch(e->type ())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void Export::save()
{

}
