#include "exportdialog.h"
#include "ui_export.h"

#include <QFileDialog>


ExportDialog::ExportDialog(const FontInfo &_fontInfo,
                           const QList<SymbolTableItem *> &_items,
                           QWidget *parent)
    : QDialog(parent), ui(new Ui::Export), fontInfo(_fontInfo), items(_items)
{
    ui->setupUi (this);
    ui->fontName->setText(QString("%1%2_%3pt_%4").arg(_fontInfo.family)
                          .arg(FontInfo::Mode::Antialias == _fontInfo.mode ? "_AA" : nullptr)
                          .arg(_fontInfo.poinSize).arg(_fontInfo.styleName)
                          .replace(" ", "_"));

    if (FontInfo::Mode::Bitmap == _fontInfo.mode)
    {
        connect(ui->cancel,  &QPushButton::clicked, this,  &ExportDialog::reject);
        connect(ui->save, &QPushButton::clicked, this, &ExportDialog::save);

        connect(ui->rbCxx11, &QRadioButton::toggled, this , [&] ()
        {
            on_btUpdate_clicked();
        });
        connect(ui->rbCxx14, &QRadioButton::toggled, this , [&] ()
        {
            on_btUpdate_clicked();
        });
    }
    else
        ui->rbFrame->setVisible(false);

    on_btUpdate_clicked();
}
ExportDialog::~ExportDialog()
{
    delete ui;
}

void ExportDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type ())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

IFontExport::CxxStandart ExportDialog::cxxStandart()
{
    return ui->rbCxx11->isChecked() ? IFontExport::CxxStandart::Cxx11 :
           IFontExport::CxxStandart::Cxx14;
}

void ExportDialog::convert()
{
    IFontExport *exp;

    if (FontInfo::Mode::Bitmap == fontInfo.mode)
        exp = new BitColor(ui->fontName->text(), items,  cxxStandart());
    else
        exp = new GrayscaleColor(ui->fontName->text(), items);
    QString str = exp->process();
    ui->plainTextEdit->setPlainText(str);

    delete exp;
}

void ExportDialog::on_btUpdate_clicked()
{
    convert();
}

void ExportDialog::save()
{
    QString fontName(ui->fontName->text());

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    fontName + ".h",
                                                    tr("C/C++ Header file (*.h *.hh *.hpp)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    fontName = fontName.toUpper();
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "#ifndef " <<  fontName << "_H_" << endl
        << "#define " << fontName << "_H_"
        << endl << endl
        << "#include <stdint.h>" << endl << endl
        << ui->plainTextEdit->toPlainText()
        << endl << endl
        << "#endif /* " <<  fontName << "_H_ */";
}
