#ifndef ASCIISERVICE_H
#define ASCIISERVICE_H

#include <QString>
#include <QListWidget>

class AsciiService
{
public:
    static int toAsciiCode(int value);
    static QString describe(int value);
};

#endif // ASCIISERVICE_H