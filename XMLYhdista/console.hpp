#pragma once

#include <QObject>
#include <QSocketNotifier>
#include <iostream>
#include <../../XMLTVp/programme.h>

class Console : public QObject
{
    Q_OBJECT;

public:
    Console();

    void run();

signals:
    void quit();

private:
    QSocketNotifier *m_notifier;
    std::vector<xmltvp::xmltvprogramme> programmes;

private slots:
    void readCommand();
};

inline Console::Console()
{
    m_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);
}
