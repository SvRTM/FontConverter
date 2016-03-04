#include "export.h"
#include "fontexport.h"

#include "ui_export.h"


Export::Export(const QList<TableItem *> &items, QWidget *parent)
    : QDialog(parent), ui(new Ui::Export)
{
    ui->setupUi (this);

    connect(ui->cancel,  &QPushButton::clicked, this,  &Export::reject);
    connect(ui->save, &QPushButton::clicked, this, &Export::save);

    IFontExport *exp = new BitColor(items);
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
