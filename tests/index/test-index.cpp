#include "test-index.h"
#include "QZeitgeist/QZeitgeist"
#include "QZeitgeist/TimeRange"
#include "QZeitgeist/ResultSet"
#include "QZeitgeist/Index"

#include <QTest>
#include <QDataStream>
#include <QDebug>
#include <QSignalSpy>

using namespace QZeitgeist;

void IndexTest::initTestCase()
{
    m_log = Log::defaultLog();

    QSignalSpy errorSpy(m_log, SIGNAL(error(int,QString)));
    QSignalSpy okSpy(m_log, SIGNAL(eventsInserted(int,QList<quint32>)));

    Event ev;
    ev.setInterpretation(QUrl("foo://Interp"));
    ev.setManifestation(QUrl("foo://Manif"));
    ev.setActor(QUrl("app://firefox.desktop"));

    Subject su;
    su.setUrl(QUrl("file:///tmp/bar.txt"));
    su.setInterpretation(QUrl("foo://TextDoc"));
    su.setManifestation(QUrl("foo://File"));
    su.setMimeType("text/plain");
    su.setOrigin(QUrl("file:///tmp"));
    su.setText("bar.txt");

    ev.addSubject(su);

    m_log->insertEvent(ev);

    while (errorSpy.count() == 0 && okSpy.count() == 0)
        QTest::qWait(50);

    if (errorSpy.count() > 0)
        QFAIL(qPrintable("Error: " + errorSpy.first().at(1).toString()));

    QList<quint32> ids = okSpy.first().at(1).value<QList<quint32> >();

    m_id = ids.first();
}

void IndexTest::cleanupTestCase()
{
    m_log->deleteEvent(m_id);
    delete m_log;
}

void IndexTest::basicTest()
{
    ResultSet tmp; // For qRegisterMetaType
    Index *index = new Index;

    QSignalSpy errorSpy(index, SIGNAL(error(QString)));
    QSignalSpy okSpy(index, SIGNAL(eventsFound(QZeitgeist::ResultSet)));

    QList<Event> templates;

    index->search("bar.txt", TimeRange::timeRangeAnytime(), templates,
                  0, 1, MostRecentEventsResult);

    while (errorSpy.count() == 0 && okSpy.count() == 0)
        QTest::qWait(50);

    if (errorSpy.count() > 0)
        QFAIL(qPrintable("Error: " + errorSpy.first().at(0).toString()));

    ResultSet rs = okSpy.first().at(0).value<ResultSet>();
    QVERIFY(rs.hasNext());

    Event ev = rs.nextValue();

    QCOMPARE(QUrl("foo://Interp"), ev.interpretation());
    QCOMPARE(QUrl("foo://Manif"), ev.manifestation());
    QCOMPARE(QUrl("app://firefox.desktop"), ev.actor());
    QVERIFY(ev.subjects().count() == 1);
    QCOMPARE(QString("bar.txt"), ev.subjects().first().text());
}

QTEST_MAIN(IndexTest)

