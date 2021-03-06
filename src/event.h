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
#ifndef QZEITGEIST_EVENT_H_
#define QZEITGEIST_EVENT_H_

#include <QZeitgeist/QZeitgeist>
#include <QZeitgeist/Subject>
#include <QtCore/QMetaType>
#include <QtCore/QList>

class QUrl;

namespace QZeitgeist
{

class QZEITGEIST_EXPORT Event
{
public:
    Event();
    Event(const Event &other);
    ~Event();

    Event &operator=(const Event &other);
    bool operator==(const Event &other) const;

    bool isValid() const;
    QIcon icon() const;

    quint32 id() const;
    void setId(quint32 id);

    qint64 timestamp() const;
    void setTimestamp(qint64 timestamp);

    QUrl origin() const;
    void setOrigin(const QUrl &origin);

    QUrl actor() const;
    void setActor(const QUrl &actor);

    QUrl interpretation() const;
    void setInterpretation(const QUrl &interpretation);

    QUrl manifestation() const;
    void setManifestation(const QUrl &manifestation);

    QByteArray payload() const;
    void setPayload(const QByteArray &payload);

    QList<Subject> subjects() const;
    void setSubjects(const QList<Subject> &subjects);
    void removeSubject(const Subject &subject);
    void addSubject(const Subject &subject);

    friend QZEITGEIST_EXPORT QDataStream &operator<<(QDataStream &stream, const Event &event);
    friend QZEITGEIST_EXPORT QDataStream &operator>>(QDataStream &stream, Event &event);

private:
    HANDLE createHandle() const;
    static Event fromHandle(HANDLE handle);

    void detach();

    class EventPrivate *d;

    friend class Tools;
};

} // namespace QZeitgeist

Q_DECLARE_METATYPE(QZeitgeist::Event)

#endif // QZEITGEIST_EVENT_H_
