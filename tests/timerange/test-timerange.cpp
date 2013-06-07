#include "test-timerange.h"
#include "QZeitgeist/QZeitgeist"
#include "QZeitgeist/TimeRange"

#include <QTest>

using namespace QZeitgeist;

void TimeRangeTest::streamOperators()
{
    TimeRange tr(23, 60);

    QByteArray data;
    QDataStream streamIn(&data, QIODevice::WriteOnly);
    streamIn << tr;

    QVERIFY(data.isEmpty() == false);

    TimeRange tr2;
    QDataStream streamOut(data);
    streamOut >> tr2;

    QVERIFY(tr == tr2);
}

void TimeRangeTest::create()
{
    TimeRange tr(0, 1);

    QCOMPARE(tr.start(), (qint64) 0);
    QCOMPARE(tr.end(), (qint64) 1);
}

void TimeRangeTest::isValid()
{
    QCOMPARE(TimeRange(20, 50).isValid(), true);
    QCOMPARE(TimeRange(20, 20).isValid(), true);
    QCOMPARE(TimeRange(20, 19).isValid(), false);
    QCOMPARE(TimeRange(-20, 50).isValid(), false);
    QCOMPARE(TimeRange(-1, -1).isValid(), false);
    QCOMPARE(TimeRange(0, 0).isValid(), true);
    QCOMPARE(TimeRange(0, 200).isValid(), true);
}

void TimeRangeTest::intersect()
{
    TimeRange tr1(20, 40);
    TimeRange tr2(25, 45);
    TimeRange tr3(45, 50);
    TimeRange tr4(41, 60);

    QCOMPARE(tr1.intersect(tr2), TimeRange(25, 40));
    QCOMPARE(tr3.intersect(tr1).isValid(), false);
    QCOMPARE(tr3.intersect(tr4), TimeRange(45, 50));
    QCOMPARE(tr4.intersect(tr3), TimeRange(45, 50));
}

QTEST_MAIN(TimeRangeTest)

