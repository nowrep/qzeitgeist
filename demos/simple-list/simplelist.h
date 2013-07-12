#include <QZeitgeist/Log>
#include <QZeitgeist/TimeRange>
#include <QZeitgeist/ResultSet>
#include <QZeitgeist/Event>
#include <QZeitgeist/Subject>
#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtCore/QObject>
#include <stdio.h>

using namespace QZeitgeist;

class SimpleList : public QObject
{
    Q_OBJECT

public:
    SimpleList();

private Q_SLOTS:
    void slotEventsFound(int id, QZeitgeist::ResultSet rs);
    void slotError(int id, const QString &msg);
};

