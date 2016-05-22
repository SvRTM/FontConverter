/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "Widgets/characterInfoTableItem.h"
#include "font/fontExport.h"

#include <QList>
#include <QDialog>

namespace Ui
{
class ExportDialog;
}

struct FontInfo
{
    const FontMode mode;
    const QString &family;
    const int pointSize;
    const QString &styleName;
};

class ExportDialog final : public QDialog
{
        Q_OBJECT

    public:
        explicit ExportDialog(const FontInfo &fontInfo,
                              const QList<CharacterInfoTableItem *> &items, QWidget *pParent);
        virtual ~ExportDialog();

    private:
        IFontExport::CxxStandart cxxStandart() const;
        void convert();

        void changeEvent(QEvent *e);

    private slots:
        void save();
        void on_saveIfont_clicked();
        void on_btUpdate_clicked();

    private:
        Ui::ExportDialog *ui;
        const FontInfo &fontInfo;
        const QList<CharacterInfoTableItem *> &items;
};

#endif // EXPORTDIALOG_H
