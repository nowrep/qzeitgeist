#include "simplelist.h"

using namespace QZeitgeist;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    new SimpleList;

    return app.exec();
}
