#include <QObject>

class TimeRangeTest : public QObject
{
    Q_OBJECT

private slots:
    void create();
    void isValid();
    void intersect();
};

