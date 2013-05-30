#include "test-symbols.h"
#include "QZeitgeist/QZeitgeist"
#include "QZeitgeist/Symbol"

#include <QTest>

using namespace QZeitgeist;

void SymbolTest::sharedData()
{
    // It shouldn't leak and crash
    // How to test for leaks with QtTest?

    Symbol s1(QUrl("http://foo.bar"));
    Symbol s2(QUrl("http://bar.com"));

    s2 = s1;

    QCOMPARE(s1.url(), s2.url());

    Symbol s3(QUrl("http://zeitgeist.org"));
    Symbol s4(s3);

    QCOMPARE(s3.url(), s4.url());
}

void SymbolTest::nullSymbols()
{
    QUrl nullUrl;
    Symbol s1(nullUrl);

    QCOMPARE(s1.isA(nullUrl), false);
    QCOMPARE(s1.isA(QUrl(QZEITGEIST_NFO_MEDIA)), false);

    Symbol s2(QUrl(QZEITGEIST_NFO_MEDIA));

    QCOMPARE(s2.isA(nullUrl), false);
}

void SymbolTest::notUriSymbols()
{
    Symbol s1(QUrl("first"));

    // not uri
    QCOMPARE(s1.isA(QUrl("second")), false);

    // equal, but not uri
    QCOMPARE(s1.isA(QUrl("first")), false);

}

void SymbolTest::urisEqual()
{
    Symbol s1(QUrl(QZEITGEIST_NFO_AUDIO));

    QCOMPARE(s1.isA(QUrl(QZEITGEIST_NFO_AUDIO)), true);
}

void SymbolTest::urisInheritance()
{
    Symbol sVector(QUrl(QZEITGEIST_NFO_VECTOR_IMAGE));
    Symbol sMedia(QUrl(QZEITGEIST_NFO_MEDIA));

    QCOMPARE(sVector.isA(QUrl(QZEITGEIST_NFO_MEDIA)), true);
    QCOMPARE(sMedia.isA(QUrl(QZEITGEIST_NFO_VECTOR_IMAGE)), false);
    QCOMPARE(sMedia.isA(QUrl(QZEITGEIST_NFO_SOFTWARE)), false);
}

static void checkUris(const QList<QUrl> &uris)
{
    QString validUri = "http://www.semanticdesktop.org/ontologies";

    foreach (const QUrl &uri, uris) {
        QCOMPARE(uri.toString().startsWith(validUri), true);
    }
}

void SymbolTest::urisValid()
{
    Symbol sMedia(QUrl(QZEITGEIST_NFO_MEDIA));
    Symbol sVector(QUrl(QZEITGEIST_NFO_VECTOR_IMAGE));
    Symbol sSoftware(QUrl(QZEITGEIST_NFO_SOFTWARE));

    checkUris(sMedia.children());
    checkUris(sMedia.parents());

    checkUris(sVector.children());
    checkUris(sVector.parents());

    checkUris(sSoftware.children());
    checkUris(sSoftware.parents());
}

QTEST_MAIN(SymbolTest)

