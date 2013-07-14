#include <QtGui/QApplication>
#include "zgbrowser.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    ZgBrowser browser;
    browser.show();
    return app.exec();
}
