#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

#include "symboltableitem.h"
#include <QList>
#include <QDialog>

namespace Ui
{
    class Export;
}

class ExportDialog : public QDialog
{
        Q_OBJECT

    public:
        explicit ExportDialog(const int depth, const QList<SymbolTableItem *> &items, QWidget *parent);
        virtual ~ExportDialog();

    private:
        void changeEvent(QEvent *e);

    private slots:
        void save();

    private:
        Ui::Export *ui;
};

#endif // EXPORTDIALOG_H
