#include <QObject>

class SymbolTest : public QObject
{
    Q_OBJECT

private slots:
    void sharedData();

    void nullSymbols();
    void notUriSymbols();
    void urisEqual();
    void urisInheritance();
    void urisValid();
};

