#include "simplelist.h"

SimpleList::SimpleList() : QObject()
{
    // Match anytime timerange
    TimeRange timeRange = TimeRange::timeRangeAnytime();

    // Empty event matches anything
    QList<Event> eventTemplates;

    Log *log = new Log();
    connect(log, SIGNAL(eventsFound(int,QZeitgeist::ResultSet)),
            this, SLOT(slotEventsFound(int,QZeitgeist::ResultSet)));
    connect(log, SIGNAL(error(int,QString)),
            this, SLOT(slotError(int,QString)));

    log->findEvents(timeRange, eventTemplates, AnyState, MostRecentEventsResult, 10);
}

void SimpleList::slotEventsFound(int id, QZeitgeist::ResultSet rs)
{
    QTextStream cout(stdout, QIODevice::WriteOnly);

    while (rs.hasNext()) {
        Event evt = rs.nextValue();
        cout << "Event #" << evt.id() << ":" << "\n";
        cout << "\tWhen: " << evt.timestamp() << "\n";
        cout << "\tActor: " << evt.actor().toString() << "\n";
        cout << "\tOrigin: " << evt.origin().toString() << "\n";
        cout << "\tInterpretaton: " << evt.interpretation().toString() << "\n";
        cout << "\tManifestation: " << evt.manifestation().toString() << "\n";
        foreach (const Subject &subj, evt.subjects()) {
            cout << "\tText: " << subj.text() << "\n";
            cout << "\tURI: " << subj.url().toString() << "\n";
            cout << "\tInterpretaton: " << subj.interpretation().toString() << "\n";
            cout << "\tManifestation: " << subj.manifestation().toString() << "\n";
            cout << "\tMime type: " << subj.mimeType() << "\n";
        }
    }

    QCoreApplication::quit();
}

void SimpleList::slotError(int id, const QString &msg)
{
    QTextStream cout(stderr, QIODevice::WriteOnly);
    cout << "Error: " << msg;

    QCoreApplication::quit();
}

