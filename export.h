#ifndef EXPORT_H
#define EXPORT_H

#include "tableitem.h"
#include <QList>
#include <QDialog>

namespace Ui
{
    class Export;
}

class Export : public QDialog
{
        Q_OBJECT

    public:
        explicit Export(const QList<TableItem *> &items, QWidget *parent);
        virtual ~Export();

    private:
        void changeEvent(QEvent *e);

    private slots:
        void save();

    private:
        Ui::Export *ui;
};

#endif // EXPORT_H
