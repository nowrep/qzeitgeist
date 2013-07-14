#include "zgbrowser.h"
#include "zgbrowser_ui.h"
#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>

#include <QZeitgeist/TimeRange>

using namespace QZeitgeist;

ZgBrowser::ZgBrowser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ZgBrowser)
{
    ui->setupUi(this);
    ui->splitter->setSizes(QList<int>() << 500 << 500);

    ui->webView->load(QUrl("http://zeitgeist-project.com"));

    m_log = Log::defaultLog();

    connect(m_log, SIGNAL(eventsFound(int,QZeitgeist::ResultSet)),
            this, SLOT(slotEventsFound(int,QZeitgeist::ResultSet)));
    connect(m_log, SIGNAL(error(int,QString)),
            this, SLOT(slotError(int,QString)));

    TimeRange timeRange = TimeRange::timeRangeAnytime();
    QList<Event> eventTemplates;
    eventTemplates << Event();
    m_log->findEvents(timeRange, eventTemplates, AnyState, MostRecentEventsResult, 100);

    m_monitor = new Monitor(timeRange, eventTemplates, this);
    m_log->installMonitor(m_monitor);

    connect(m_monitor, SIGNAL(eventsInserted(QZeitgeist::TimeRange,QZeitgeist::ResultSet)),
            this, SLOT(slotEventsInserted(QZeitgeist::TimeRange,QZeitgeist::ResultSet)));

    connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
            this, SLOT(itemChanged(QListWidgetItem*)));

    connect(ui->webView, SIGNAL(loadFinished(bool)), this, SLOT(logPage()));
}

void ZgBrowser::slotEventsFound(int id, QZeitgeist::ResultSet rs)
{
    Q_UNUSED(id)

    while (rs.hasNext()) {
        Event evt = rs.nextValue();

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(evt.subjects().first().text());
        item->setIcon(evt.icon());

        QVariant v;
        v.setValue<Event>(evt);
        item->setData(Qt::UserRole + 10, v);

        ui->listWidget->addItem(item);
    }

    ui->listWidget->setCurrentRow(0);
}

void ZgBrowser::slotError(int id, const QString &msg)
{
    QMessageBox::critical(this, "Error", "Cannot retrive events: " + msg);
}

void ZgBrowser::slotEventsInserted(const QZeitgeist::TimeRange &tr, QZeitgeist::ResultSet rs)
{
    while (rs.hasNext()) {
        Event evt = rs.nextValue();

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(evt.subjects().first().text());
        item->setIcon(evt.icon());

        QVariant v;
        v.setValue<Event>(evt);
        item->setData(Qt::UserRole + 10, v);

        ui->listWidget->insertItem(0, item);
    }
}

void ZgBrowser::itemChanged(QListWidgetItem *item)
{
    if (!item) {
        return;
    }

    Event evt = item->data(Qt::UserRole + 10).value<Event>();
    Subject subj = evt.subjects().first();

    ui->id->setText(QString::number(evt.id()));
    ui->date->setText(QDateTime::fromMSecsSinceEpoch(evt.timestamp()).toString());
    ui->actor->setText(evt.actor().toString());
    ui->manifestation->setText(evt.manifestation().toString());
    ui->interpretation->setText(evt.interpretation().toString());
    ui->text->setText(subj.text());
    ui->url->setText(subj.url().toString());

    if (subj.interpretation() == QZEITGEIST_NFO_WEBSITE) {
        ui->webView->load(subj.url());
    }
}

void ZgBrowser::logPage()
{
    if (ui->webView->title().isEmpty()) {
        return;
    }

    Event event;
    event.setActor(QUrl("applicaton://zeitgeist-browser-demo.desktop"));
    event.setInterpretation(QZEITGEIST_ZG_ACCESS_EVENT);
    event.setManifestation(QZEITGEIST_ZG_USER_ACTIVITY);

    Subject subject;
    subject.setInterpretation(QZEITGEIST_NFO_WEBSITE);
    subject.setManifestation(QZEITGEIST_NFO_REMOTE_DATA_OBJECT);
    subject.setUrl(ui->webView->url());
    subject.setText(ui->webView->title());

    event.addSubject(subject);
    m_log->insertEvent(event);

    qDebug() << "Logged" << subject.url();
}
