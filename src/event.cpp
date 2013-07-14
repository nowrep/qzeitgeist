/*
 * This file is part of QZeitgeist.
 *
 * Copyright (C) 2013 David Rosca <nowrep@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
extern "C" {
#include <zeitgeist.h>
}

#include "event.h"
#include "tools.h"
#include <QtCore/QUrl>
#include <QtCore/QAtomicInt>
#include <QtCore/QDebug>
#include <QtGui/QFileIconProvider>

namespace QZeitgeist
{

class EventPrivate
{
public:
    explicit EventPrivate();
    EventPrivate(const EventPrivate &other);
    bool compare(const EventPrivate &other) const;

    QAtomicInt ref;
    quint32 id;
    qint64 timestamp;
    QUrl origin;
    QUrl actor;
    QUrl interpretation;
    QUrl manifestation;
    QByteArray payload;
    QList<Subject> subjects;
};

EventPrivate::EventPrivate()
    : ref(1)
    , id(0)
    , timestamp(0)
{
}

EventPrivate::EventPrivate(const EventPrivate &other)
    : ref(1)
    , id(other.id)
    , timestamp(other.timestamp)
    , origin(other.origin)
    , actor(other.actor)
    , interpretation(other.interpretation)
    , manifestation(other.manifestation)
    , payload(other.payload)
    , subjects(other.subjects)
{
}

bool EventPrivate::compare(const EventPrivate &other) const
{
    return id == other.id
           && timestamp == other.timestamp
           && origin == other.origin
           && actor == other.actor
           && interpretation == other.interpretation
           && manifestation == other.manifestation
           && payload == other.payload
           && subjects == other.subjects;
}

// class Event
Event::Event()
    : d(new EventPrivate)
{
    qRegisterMetaType<Event>("QZeitgeist::Event");
}

Event::Event(const Event &other)
    : d(other.d)
{
    d->ref.ref();
}

Event::~Event()
{
    if (!d->ref.deref()) {
        delete d;
    }
}

Event &Event::operator=(const Event &other)
{
    qAtomicAssign(d, other.d);
    return *this;
}

bool Event::operator==(const Event &other) const
{
    return d->compare(*other.d);
}

bool Event::isValid() const
{
    return d->id > 0 && d->timestamp > 0;
}

QIcon Event::icon() const
{
    QFileIconProvider icons;
    QIcon ret;

    Q_FOREACH(Subject subject, d->subjects) {
        QUrl url = subject.url();
        if (url.scheme() == "file") {
            QFileInfo info(url.path());
            ret = icons.icon(info);
            if (!ret.isNull()) {
                break;
            }
        }
    }

    if (ret.isNull()) {
        QString desktopFile = d->actor.authority().section('.', 0, 0);
        ret = QIcon::fromTheme(desktopFile);
    }

    return ret;
}

quint32 Event::id() const
{
    return d->id;
}

void Event::setId(quint32 id)
{
    detach();

    d->id = id;
}

qint64 Event::timestamp() const
{
    return d->timestamp;
}

void Event::setTimestamp(qint64 timestamp)
{
    detach();

    d->timestamp = timestamp;
}

QUrl Event::origin() const
{
    return d->origin;
}

void Event::setOrigin(const QUrl &origin)
{
    detach();

    d->origin = origin;
}

QUrl Event::actor() const
{
    return d->actor;
}

void Event::setActor(const QUrl &actor)
{
    detach();

    d->actor = actor;
}

QUrl Event::interpretation() const
{
    return d->interpretation;
}

void Event::setInterpretation(const QUrl &interpretation)
{
    detach();

    d->interpretation = interpretation;
}

QUrl Event::manifestation() const
{
    return d->manifestation;
}

void Event::setManifestation(const QUrl &manifestation)
{
    detach();

    d->manifestation = manifestation;
}

QByteArray Event::payload() const
{
    return d->payload;
}

void Event::setPayload(const QByteArray &payload)
{
    detach();

    d->payload = payload;
}

QList<Subject> Event::subjects() const
{
    return d->subjects;
}

void Event::setSubjects(const QList<Subject> &subjects)
{
    detach();

    d->subjects = subjects;
}

void Event::removeSubject(const Subject &subject)
{
    detach();

    d->subjects.removeOne(subject);
}

void Event::addSubject(const Subject &subject)
{
    detach();

    d->subjects.append(subject);
}

static const int streamVersion = 1;

QDataStream &operator<<(QDataStream &stream, const Event &event)
{
    stream << streamVersion;
    stream << event.d->id;
    stream << event.d->timestamp;
    stream << event.d->origin;
    stream << event.d->actor;
    stream << event.d->interpretation;
    stream << event.d->manifestation;
    stream << event.d->payload;
    stream << event.d->subjects;

    return stream;
}

QDataStream &operator>>(QDataStream &stream, Event &event)
{
    int version = -1;
    stream >> version;

    if (version != streamVersion) {
        qWarning() << "Event: Invalid stream version!";
        return stream;
    }

    stream >> event.d->id;
    stream >> event.d->timestamp;
    stream >> event.d->origin;
    stream >> event.d->actor;
    stream >> event.d->interpretation;
    stream >> event.d->manifestation;
    stream >> event.d->payload;
    stream >> event.d->subjects;

    return stream;
}

HANDLE Event::createHandle() const
{
    QByteArray originData = d->origin.toString().toUtf8();
    QByteArray actorData = d->actor.toString().toUtf8();
    QByteArray interpretationData = d->interpretation.toString().toUtf8();
    QByteArray manifestationData = d->manifestation.toString().toUtf8();
    GByteArray *payload = Tools::convertToByteArray(d->payload);
    GPtrArray *subjects = Tools::subjectsToPtrArray(d->subjects);

    ZeitgeistEvent *event = zeitgeist_event_new();
    zeitgeist_event_set_id(event, d->id);
    zeitgeist_event_set_timestamp(event, d->timestamp);
    zeitgeist_event_set_origin(event, originData.constData());
    zeitgeist_event_set_actor(event, actorData.constData());
    zeitgeist_event_set_interpretation(event, interpretationData.constData());
    zeitgeist_event_set_manifestation(event, manifestationData.constData());
    zeitgeist_event_set_payload(event, payload);
    zeitgeist_event_set_subjects(event, subjects);

    g_byte_array_unref(payload);
    g_ptr_array_unref(subjects);

    return event;
}

// static
Event Event::fromHandle(HANDLE handle)
{
    ZeitgeistEvent *event = (ZeitgeistEvent *)handle;
    Event ev;

    ev.d->id = zeitgeist_event_get_id(event);
    ev.d->timestamp = zeitgeist_event_get_timestamp(event);
    ev.d->origin = QUrl(zeitgeist_event_get_origin(event));
    ev.d->actor = QUrl(zeitgeist_event_get_actor(event));
    ev.d->interpretation = QUrl(zeitgeist_event_get_interpretation(event));
    ev.d->manifestation = QUrl(zeitgeist_event_get_manifestation(event));
    ev.d->payload = Tools::convertFromByteArray(zeitgeist_event_get_payload(event));
    ev.d->subjects = Tools::subjectsFromPtrArray(zeitgeist_event_get_subjects(event));

    return ev;
}

void Event::detach()
{
    qAtomicDetach(d);
}

} // namespace QZeitgeist

