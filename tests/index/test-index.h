#include <QObject>
#include "QZeitgeist/QZeitgeist"
#include "QZeitgeist/Log"

class IndexTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void basicTest();

private:
    QZeitgeist::Log *m_log;
    QList<quint32> m_ids;
};

