#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "../Widgets/symboltableitem.h"
#include "../font/fontexport.h"

#include <QList>
#include <QDialog>

namespace Ui
{
    class Export;
}

struct FontInfo
{
    enum class Mode
    {
        Antialias,
        Bitmap
    } const mode;

    const QString &family;
    const int poinSize;
    const QString &styleName;
};

class ExportDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ExportDialog(const FontInfo &_fontInfo, const QList<SymbolTableItem *> &_items,
                              QWidget *parent);
        virtual ~ExportDialog();

    private:
        IFontExport::CxxStandart cxxStandart();
        void convert();

        void changeEvent(QEvent *e);

    private slots:
        void save();


        void on_btUpdate_clicked();

    private:
        Ui::Export *ui;
        const FontInfo &fontInfo;
        const QList<SymbolTableItem *> &items;
};

#endif // EXPORTDIALOG_H
