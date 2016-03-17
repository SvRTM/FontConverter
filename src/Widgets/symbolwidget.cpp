#include "symbolwidget.h"
#include "ui_symbolwidget.h"

SymbolWidget::SymbolWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::SymbolWidget)
{
    ui->setupUi(this);

    connect(ui->sliderZoom, &QSlider::valueChanged, this, [&] (int value)
    {
        ui->lbZoom->setText(QString("x%1").arg(value));
        emit valueChanged(value);
    });

    ui->sliderZoom->setValue(10);
}
SymbolWidget::~SymbolWidget()
{
    delete ui;
}

int SymbolWidget::zoom() const
{
    return ui->sliderZoom->value();
}

void SymbolWidget::drawSize(int width, int height)
{
    ui->lbSize->setText(QString("%1x%2").arg(width).arg(height));
}

void SymbolWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}
