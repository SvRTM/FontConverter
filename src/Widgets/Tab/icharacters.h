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
