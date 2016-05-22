/*
 * Copyright © 2016 Artem Smirnov
 * All right reserved.
 * Author: Artem Smirnov <msvrtm@gmail.com>
 * License: http://www.gnu.org/licenses/gpl.txt
 */

#ifndef ICHARACTERS_H
#define ICHARACTERS_H

#include <QObject>

class ICharacters
{
    public:
        virtual const QString getCharacters() = 0;
};

Q_DECLARE_INTERFACE(ICharacters, "ICharacters")

#endif // ICHARACTERS_H
