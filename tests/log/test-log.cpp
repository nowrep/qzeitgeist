#include "test-log.h"
#include "QZeitgeist/ResultSet"
#include "QZeitgeist/TimeRange"
#include "QZeitgeist/Monitor"

#include <QTest>
#include <QSignalSpy>
#include <QDebug>

using namespace QZeitgeist;

void LogTest::initTestCase()
{
    m_log = new Log;

    m_ev.setInterpretation(QUrl("foo://Interp"));
    m_ev.setManifestation(QUrl("foo://Manif"));
    m_ev.setActor(QUrl("app://firefox.desktop"));

    m_su.setUrl(QUrl("file:///tmp/bar.txt"));
    m_su.setInterpretation(QUrl("foo://TextDoc"));
    m_su.setManifestation(QUrl("foo://File"));
    m_su.setMimeType("text/plain");
    m_su.setOrigin(QUrl("file:///tmp"));
    m_su.setText("bar.txt");
    m_su.setStorage("bfb486f6-f5f8-4296-8871-0cc749cf8ef7");

    m_ev.addSubject(m_su);
}

void LogTest::cleanupTestCase()
{
    delete m_log;
}

void LogTest::insertEvents()
{
    QSignalSpy errorSpy(m_log, SIGNAL(error(int,QString)));
    QSignalSpy okSpy(m_log, SIGNAL(eventsInserted(int,QList<quint32>)));

    int requestId = m_log->insertEvent(m_ev);

    while (errorSpy.count() == 0 && okSpy.count() == 0)
        QTest::qWait(50);

    if (errorSpy.count() > 0)
        QFAIL(qPrintable("Error: " + errorSpy.first().at(1).toString()));

    int returnRequestId = okSpy.first().at(0).toInt();
    QList<quint32> ids = okSpy.first().at(1).value<QList<quint32> >();

    QCOMPARE(returnRequestId, requestId);
    QCOMPARE(ids.count(), 1);

    m_insertedId = ids.first();
}

void LogTest::getEvents()
{
    ResultSet tmp; // For qRegisterMetaType

    QSignalSpy errorSpy(m_log, SIGNAL(error(int,QString)));
    QSignalSpy okSpy(m_log, SIGNAL(eventsGotById(int,QZeitgeist::ResultSet)));

    int requestId = m_log->getEventsByIds(QList<quint32>() << m_insertedId);

    while (errorSpy.count() == 0 && okSpy.count() == 0)
        QTest::qWait(50);

    if (errorSpy.count() > 0)
        QFAIL(qPrintable("Error: " + errorSpy.first().at(1).toString()));

    int returnRequestId = okSpy.first().at(0).toInt();
    ResultSet res = okSpy.first().at(1).value<ResultSet>();

    QCOMPARE(returnRequestId, requestId);
    QCOMPARE(1, res.size());

    Event ev = res.nextValue();

    QCOMPARE(m_ev.origin(), ev.origin());
    QCOMPARE(m_ev.actor(), ev.actor());
    QCOMPARE(m_ev.interpretation(), ev.interpretation());
    QCOMPARE(m_ev.manifestation(), ev.manifestation());
    QCOMPARE(m_ev.payload(), ev.payload());

    QCOMPARE(m_ev.subjects().first().url(), ev.subjects().first().url());
    QCOMPARE(m_ev.subjects().first().origin(), ev.subjects().first().origin());
    QCOMPARE(m_ev.subjects().first().interpretation(), ev.subjects().first().interpretation());
    QCOMPARE(m_ev.subjects().first().manifestation(), ev.subjects().first().manifestation());
    QCOMPARE(m_ev.subjects().first().text(), ev.subjects().first().text());
    QCOMPARE(m_ev.subjects().first().storage(), ev.subjects().first().storage());
    QCOMPARE(m_ev.subjects().first().mimeType(), ev.subjects().first().mimeType());
}

void LogTest::deleteEvents()
{
    TimeRange tmp; // For qRegisterMetaType

    QSignalSpy errorSpy(m_log, SIGNAL(error(int,QString)));
    QSignalSpy okSpy(m_log, SIGNAL(eventsDeleted(int,QZeitgeist::TimeRange)));

    int requestId = m_log->deleteEvents(QList<quint32>() << m_insertedId);

    while (errorSpy.count() == 0 && okSpy.count() == 0)
        QTest::qWait(50);

    if (errorSpy.count() > 0)
        QFAIL(qPrintable("Error: " + errorSpy.first().at(1).toString()));

    int returnRequestId = okSpy.first().at(0).toInt();

    QCOMPARE(returnRequestId, requestId);
}

void LogTest::installMonitor()
{
    QList<Event> templates;

    Monitor *m = new Monitor(TimeRange::timeRangeAnytime(), templates);
    m->setEventTemplates(templates);

    QCOMPARE(m_log->installMonitor(m), true);
    QCOMPARE(m_log->removeMonitor(m), true);
}

QTEST_MAIN(LogTest)

