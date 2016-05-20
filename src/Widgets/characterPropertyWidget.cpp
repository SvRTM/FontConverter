#include "characterPropertyWidget.h"
#include "ui_characterPropertyWidget.h"

CharacterPropWidget::CharacterPropWidget(QWidget *pParent)
    : QWidget(pParent)
    , ui(new Ui::CharacterPropWidget)
{
    ui->setupUi(this);

    connect(ui->sliderZoom, &QSlider::valueChanged, this, [&] (int value)
    {
        ui->lbZoom->setText(QString("x%1").arg(value));
        emit valueChanged(value);
    });

    ui->sliderZoom->setValue(10);
}
CharacterPropWidget::~CharacterPropWidget()
{
    delete ui;
}

void CharacterPropWidget::changeEvent(QEvent *e)
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

int CharacterPropWidget::zoom() const
{
    return ui->sliderZoom->value();
}

void CharacterPropWidget::drawSize(int width, int height)
{
    ui->lbSize->setText(QString("%1x%2").arg(width).arg(height));
}
