#include "test-event.h"
#include "QZeitgeist/QZeitgeist"
#include "QZeitgeist/Event"

#include <QTest>
#include <QDataStream>
#include <QDebug>

using namespace QZeitgeist;

void EventTest::sharedData()
{
    // It shouldn't leak and crash
    // How to test for leaks with QtTest?

    Event ev1;
    ev1.setInterpretation(QZEITGEIST_ZG_ACCESS_EVENT);
    ev1.setManifestation(QZEITGEIST_ZG_USER_ACTIVITY);
    ev1.setActor(QUrl("application://firefox.desktop"));

    QCOMPARE(ev1.interpretation(), QZEITGEIST_ZG_ACCESS_EVENT);

    Event ev2(ev1);
    QCOMPARE(ev2.interpretation(), QZEITGEIST_ZG_ACCESS_EVENT);

    ev2.setInterpretation(QZEITGEIST_ZG_USER_ACTIVITY);

    QCOMPARE(ev2.interpretation(), QZEITGEIST_ZG_USER_ACTIVITY);
    QCOMPARE(ev1.interpretation(), QZEITGEIST_ZG_ACCESS_EVENT);

    Event ev3;
    ev3 = ev1;

    QCOMPARE(ev3.interpretation(), QZEITGEIST_ZG_ACCESS_EVENT);
}

void EventTest::streamOperators()
{
    Event ev;
    ev.setInterpretation(QZEITGEIST_ZG_ACCESS_EVENT);
    ev.setManifestation(QZEITGEIST_ZG_USER_ACTIVITY);
    ev.setActor(QUrl("application://firefox.desktop"));

    Subject s1;
    s1.setUrl(QUrl("http://exampl.com"));
    s1.setInterpretation(QZEITGEIST_NFO_WEBSITE);
    s1.setManifestation(QZEITGEIST_NFO_REMOTE_DATA_OBJECT);
    s1.setMimeType("text/html");
    s1.setOrigin(QUrl("http://example.com"));
    s1.setText("example.com");
    s1.setStorage("net");

    Subject s2;

    ev.addSubject(s1);
    ev.addSubject(s2);

    QByteArray data;
    QDataStream streamIn(&data, QIODevice::WriteOnly);
    streamIn << ev;

    QVERIFY(data.isEmpty() == false);

    Event ev2;
    QDataStream streamOut(data);
    streamOut >> ev2;

    QVERIFY(ev == ev2);
}

QTEST_MAIN(EventTest)

