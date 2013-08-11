#include <QObject>
#include "QZeitgeist/QZeitgeist"
#include "QZeitgeist/Log"

class LogTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void insertEvents();
    void getEvents();
    void deleteEvents();
    void installMonitor();
    void getNonexistantId();

private:
    QZeitgeist::Log *m_log;

    QZeitgeist::Event m_ev;
    QZeitgeist::Subject m_su;
    quint32 m_insertedId;
};

