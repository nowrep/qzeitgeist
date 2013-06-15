#include <QObject>

class EventTest : public QObject
{
    Q_OBJECT

private slots:
    void sharedData();
    void streamOperators();

};

