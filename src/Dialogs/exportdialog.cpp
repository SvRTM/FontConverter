#include "exportdialog.h"
#include "../font/fontexport.h"

#include "ui_export.h"


ExportDialog::ExportDialog(const int depth, const QList<SymbolTableItem *> &items, QWidget *parent)
    : QDialog(parent), ui(new Ui::Export)
{
    ui->setupUi (this);

    connect(ui->cancel,  &QPushButton::clicked, this,  &ExportDialog::reject);
    connect(ui->save, &QPushButton::clicked, this, &ExportDialog::save);

    IFontExport *exp;
    if (depth == 1)
        exp = new BitColor(items);
    else
        exp = new GrayscaleColor(items);
    QString str = exp->process();
    ui->plainTextEdit->setPlainText(str);

    delete exp;
}
ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::changeEvent(QEvent *e)
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

void ExportDialog::save()
{

}
