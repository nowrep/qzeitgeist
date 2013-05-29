#include <QObject>

class MimeTypeTest : public QObject
{
    Q_OBJECT

private slots:
    void mimeTextPlain();
    void mimeNone();
    void mimeRegExp();
    void schemeFile();
    void schemeNone();

    void newMime();
    void newMimeRegExp();
    void newScheme();
};

