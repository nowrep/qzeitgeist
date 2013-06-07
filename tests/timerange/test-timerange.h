#include <QObject>

class TimeRangeTest : public QObject
{
    Q_OBJECT

private slots:
    void streamOperators();
    void create();
    void isValid();
    void intersect();
};

