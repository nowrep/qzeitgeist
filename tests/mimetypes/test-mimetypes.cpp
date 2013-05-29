#include "test-mimetypes.h"
#include "QZeitgeist/QZeitgeist"
#include "QZeitgeist/MimeType"

#include <QTest>

using namespace QZeitgeist;

void MimeTypeTest::mimeTextPlain()
{
    QUrl result = MimeType::getInterpretationUrl("text/plain");
    QUrl expected = QUrl(QZEITGEIST_NFO_TEXT_DOCUMENT);

    QCOMPARE(result, expected);
}

void MimeTypeTest::mimeNone()
{
    QUrl result = MimeType::getInterpretationUrl("asdfasdf");

    QCOMPARE(result, QUrl());
}

void MimeTypeTest::mimeRegExp()
{
    QUrl result = MimeType::getInterpretationUrl("application/x-applix-FOOOOBAR!");
    QUrl expected = QUrl(QZEITGEIST_NFO_DOCUMENT);

    QCOMPARE(result, expected);

    result = MimeType::getInterpretationUrl("application/x-applix-spreadsheet");
    expected = QUrl(QZEITGEIST_NFO_SPREADSHEET);

    QCOMPARE(result, expected);
}

void MimeTypeTest::schemeFile()
{
    QString result = MimeType::getManifestation(QUrl("file:///tmp/foo.txt"));
    QString expected = QString(QZEITGEIST_NFO_FILE_DATA_OBJECT);

    QCOMPARE(result, expected);
}

void MimeTypeTest::schemeNone()
{
    QString result = MimeType::getManifestation(QUrl("asdf://asdfasdf"));

    QCOMPARE(result, QString());
}

void MimeTypeTest::newMime()
{
    MimeType::registerMimeType("foo/bar", QUrl("http://foo.bar"));

    QCOMPARE(MimeType::getInterpretationUrl("foo/bar"), QUrl("http://foo.bar"));
    QCOMPARE(MimeType::getInterpretationUrl("foo/bars"), QUrl());
}

void MimeTypeTest::newMimeRegExp()
{
    MimeType::registerMimeType("bar/f.*", QUrl("http://bar.foo"), MimeType::RegExpString);

    QCOMPARE(MimeType::getInterpretationUrl("bar/fooo"), QUrl("http://bar.foo"));
    QCOMPARE(MimeType::getInterpretationUrl("bar/booo"), QUrl());
}

void MimeTypeTest::newScheme()
{
    MimeType::registerUrlScheme("foo://", "Foo-Bar");

    QCOMPARE(MimeType::getManifestation(QUrl("foo://bar.com/foo")), QString("Foo-Bar"));
    QCOMPARE(MimeType::getManifestation(QUrl("boo://far.com/boo")), QString());
}

QTEST_MAIN(MimeTypeTest)

