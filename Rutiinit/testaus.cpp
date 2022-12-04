#include "testaus.h"

Testaus::Testaus(QObject *parent) : QObject(parent)
{

}

void Testaus::testFoo()
{
    QVERIFY(1 == 0);
}
