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
#include <QtCore/QUrl>
#include <QtCore/QDebug>

namespace QZeitgeist
{

static GPtrArray *convertToPtrArray(const QList<Subject> &subjects)
{
    GPtrArray *array = g_ptr_array_sized_new(subjects.size());

    foreach (const Subject &subject, subjects) {
        g_ptr_array_add(array, (gpointer)subject.createHandle());
    }

    return array;
}

static QList<Subject> convertFromPtrArray(GPtrArray *array)
{
    QList<Subject> list;

    for (unsigned i = 0; i < array->len; ++i) {
        Subject subject = Subject::fromHandle(g_ptr_array_index(array, i));
        list.append(subject);
    }

    return list;
}

static GByteArray *convertToByteArray(const QByteArray &array)
{
    GByteArray *garray = g_byte_array_new();
    g_byte_array_append(garray, (guint8 *)array.constData(), array.size());
    return garray;
}

static QByteArray convertFromByteArray(GByteArray *array)
{
    return QByteArray((const char *)array->data, (int) array->len);
}

class EventPrivate
{
public:
    explicit EventPrivate();
    bool compare(const EventPrivate &other) const;
    void copy(const EventPrivate &other);

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
    : id(0)
    , timestamp(0)
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

void EventPrivate::copy(const EventPrivate &other)
{
    if (this != &other) {
        id = other.id;
        timestamp = other.timestamp;
        origin = other.origin;
        actor = other.actor;
        interpretation = other.interpretation;
        manifestation = other.manifestation;
        payload = other.payload;
        subjects = other.subjects;
    }
}

// class Event
Event::Event()
    : d(new EventPrivate)
{
}

Event::Event(const Event &other)
    : d(new EventPrivate)
{
    d->copy(*other.d);
}

Event::~Event()
{
}

Event &Event::operator=(const Event &other)
{
    d->copy(*other.d);
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

quint32 Event::id() const
{
    return d->id;
}

void Event::setId(quint32 id)
{
    d->id = id;
}

qint64 Event::timestamp() const
{
    return d->timestamp;
}

void Event::setTimestamp(qint64 timestamp)
{
    d->timestamp = timestamp;
}

QUrl Event::origin() const
{
    return d->origin;
}

void Event::setOrigin(const QUrl &origin)
{
    d->origin = origin;
}

QUrl Event::actor() const
{
    return d->actor;
}

void Event::setActor(const QUrl &actor)
{
    d->actor = actor;
}

QUrl Event::interpretation() const
{
    return d->interpretation;
}

void Event::setInterpretation(const QUrl &interpretation)
{
    d->interpretation = interpretation;
}

QUrl Event::manifestation() const
{
    return d->manifestation;
}

void Event::setManifestation(const QUrl &manifestation)
{
    d->manifestation = manifestation;
}

QByteArray Event::payload() const
{
    return d->payload;
}

void Event::setPayload(const QByteArray &payload)
{
    d->payload = payload;
}

QList<Subject> Event::subjects() const
{
    return d->subjects;
}

void Event::setSubjects(const QList<Subject> &subjects)
{
    d->subjects = subjects;
}

void Event::removeSubject(const Subject &subject)
{
    d->subjects.removeOne(subject);
}

void Event::addSubject(const Subject &subject)
{
    d->subjects.append(subject);
}

HANDLE Event::createHandle() const
{
    QByteArray originData = d->origin.toString().toUtf8();
    QByteArray actorData = d->actor.toString().toUtf8();
    QByteArray interpretationData = d->interpretation.toString().toUtf8();
    QByteArray manifestationData = d->manifestation.toString().toUtf8();
    GByteArray *payload = convertToByteArray(d->payload);
    GPtrArray *subjects = convertToPtrArray(d->subjects);

    ZeitgeistEvent *event = zeitgeist_event_new();
    zeitgeist_event_set_id(event, d->id);
    zeitgeist_event_set_timestamp(event, d->timestamp);
    zeitgeist_event_set_origin(event, originData.constData());
    zeitgeist_event_set_actor(event, actorData.constData());
    zeitgeist_event_set_interpretation(event, interpretationData.constData());
    zeitgeist_event_set_manifestation(event, manifestationData.constData());
    zeitgeist_event_set_payload(event, payload);
    zeitgeist_event_set_subjects(event, subjects);

    // Is it necessary here?
    g_byte_array_unref(payload);
    g_ptr_array_unref(subjects);
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
    ev.d->payload = convertFromByteArray(zeitgeist_event_get_payload(event));
    ev.d->subjects = convertFromPtrArray(zeitgeist_event_get_subjects(event));

    return ev;
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

}; // namespace QZeitgeist

