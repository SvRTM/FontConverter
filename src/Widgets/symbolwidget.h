#ifndef SYMBOLWIDGET_H
#define SYMBOLWIDGET_H

#include <QWidget>

namespace Ui {
    class SymbolWidget;
}
class SymbolWidget : public QWidget
{
        Q_OBJECT
    public:
        explicit SymbolWidget(QWidget *parent);
        virtual~SymbolWidget();

    public:
        int zoom() const;
        void drawSize(int width = 0, int height = 0);

    protected:
        void changeEvent(QEvent *e);

    private:
        Ui::SymbolWidget *ui;

    signals:
        void valueChanged(int value);

    public slots:
};

#endif // SYMBOLWIDGET_H
