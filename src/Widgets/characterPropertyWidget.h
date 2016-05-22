/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

#ifndef CHARACTERPROPERTYWIDGET_H
#define CHARACTERPROPERTYWIDGET_H

#include <QWidget>

namespace Ui
{
class CharacterPropWidget;
}

class CharacterPropWidget final : public QWidget
{
        Q_OBJECT

    public:
        explicit CharacterPropWidget(QWidget *pParent);
        virtual~CharacterPropWidget();

    public:
        int zoom() const;
        void drawSize(int width = 0, int height = 0);

    protected:
        void changeEvent(QEvent *e);

    private:
        Ui::CharacterPropWidget *ui;

    signals:
        void valueChanged(int value);
};

#endif // CHARACTERPROPERTYWIDGET_H
