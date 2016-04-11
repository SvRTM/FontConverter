#include "exportdialog.h"
#include "ui_export.h"

#include <QFileDialog>


ExportDialog::ExportDialog(const FontInfo &_fontInfo,
                           const QList<SymbolTableItem *> &_items, QWidget *parent)
    : QDialog(parent), ui(new Ui::Export), fontInfo(_fontInfo), items(_items)
{
    ui->setupUi (this);
    ui->fontName->setText(QString("%1%2_%3pt_%4").arg(_fontInfo.family)
                          .arg(FontMode::Antialias == _fontInfo.mode ? "_AA" : nullptr)
                          .arg(_fontInfo.poinSize).arg(_fontInfo.styleName)
                          .replace(" ", "_"));

    connect(ui->save, &QPushButton::clicked, this, &ExportDialog::save);
    connect(ui->cancel, &QPushButton::clicked, this, &ExportDialog::reject);

    connect(ui->rbMode_1, &QRadioButton::clicked, this , [&] ()
    {
        ui->saveIfont->setVisible(false);
        on_btUpdate_clicked();
    });
    connect(ui->rbMode_2, &QRadioButton::clicked, this , [&] ()
    {
        ui->saveIfont->setVisible(true);
        on_btUpdate_clicked();
    });

    if (FontMode::Bitmap == _fontInfo.mode)
    {
        connect(ui->rbCxx11, &QRadioButton::clicked, this , [&] ()
        {
            on_btUpdate_clicked();
        });
        connect(ui->rbCxx14, &QRadioButton::clicked, this , [&] ()
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
    const IFontExport::Mode mode = ui->rbMode_1->isChecked() ?
                                   IFontExport::Mode::M1 : IFontExport::Mode::M2;

    IFontExport *exp;

    if (FontMode::Bitmap == fontInfo.mode)
        exp = new BitColor(ui->fontName->text(), items,  mode, cxxStandart());
    else
        exp = new GrayscaleColor(ui->fontName->text(), items, mode);

    QPair<QString, QString> pair = exp->process();

    ui->cppFile->setPlainText(pair.first);
    ui->headerFile->setPlainText(pair.second);

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
    QFile fileH(fileName);
    if (!fileH.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    QString s = fileName.replace(QRegExp("(.*\\.)(.*)"), "\\1cpp");
    QFile fileCpp(s);
    if (!fileCpp.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream h(&fileH);
    h.setCodec("UTF-8");
    h << ui->headerFile->toPlainText();

    QTextStream cpp(&fileCpp);
    cpp.setCodec("UTF-8");
    cpp << ui->cppFile->toPlainText();
}

void ExportDialog::on_saveIfont_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                       "ifont.h",
                       tr("C/C++ Header file (*.h *.hh *.hpp)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << IFontExport::saveIFont();
}
