#ifndef ZGBROWSER_H
#define ZGBROWSER_H

#include <QtGui/QMainWindow>
#include <QZeitgeist/Log>
#include <QZeitgeist/Monitor>
#include <QZeitgeist/Event>
#include <QZeitgeist/ResultSet>
#include <QZeitgeist/TimeRange>

namespace Ui {
class ZgBrowser;
}

class QIcon;
class QListWidgetItem;

class ZgBrowser : public QWidget
{
    Q_OBJECT

public:
    ZgBrowser(QWidget *parent = 0);

private Q_SLOTS:
    void slotEventsFound(int id, QZeitgeist::ResultSet rs);
    void slotError(int id, const QString &msg);
    void slotEventsInserted(const QZeitgeist::TimeRange &tr, QZeitgeist::ResultSet rs);

    void itemChanged(QListWidgetItem *item);
    void logPage();

private:
    Ui::ZgBrowser *ui;
    QZeitgeist::Log *m_log;
    QZeitgeist::Monitor *m_monitor;
};

#endif // ZGBROWSER_H
