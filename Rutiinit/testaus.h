#ifndef TESTAUS_H
#define TESTAUS_H

#include <QObject>
#include <QtTest>


class Testaus : public QObject
{
    Q_OBJECT
public:
    explicit Testaus(QObject *parent = nullptr);

private slots:
    void testFoo();

signals:

};

#endif // TESTAUS_H
