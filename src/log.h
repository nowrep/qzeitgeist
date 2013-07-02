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
#ifndef QZEITGEIST_LOG_H_
#define QZEITGEIST_LOG_H_

#include <QZeitgeist/QZeitgeist>
#include <QZeitgeist/Event>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QUrl>

class QStringList;

namespace QZeitgeist
{

class Monitor;
class TimeRange;
class ResultSet;

class QZEITGEIST_EXPORT Log : public QObject
{
    Q_OBJECT

public:
    explicit Log(QObject *parent = 0);
    ~Log();

    bool installMonitor(Monitor *monitor);
    int removeMonitor(Monitor *monitor);

    int insertEvent(const Event &event);
    int insertEvents(const QList<Event> &events);

    int findEvents(const TimeRange &timeRange, const QList<Event> &eventTemplates,
                   StorageState storageState, ResultType resultType, int count);

    int findEventIds(const TimeRange &timeRange, const QList<Event> &eventTemplates,
                     StorageState storageState, ResultType resultType, int count);

    int getEventsByIds(const QList<quint32> &eventIds);

    int deleteEvents(const QList<quint32> &eventIds);

    int findRelatedUrls(const TimeRange &timeRange, const QList<Event> &eventTemplates,
                        const QList<Event> &resultEventTemplates, StorageState storageState,
                        RelevantResultType relevantResultType, int count);

    QString version() const;
    QString dataPath() const;
    QStringList extensions() const;

    static Log *defaultLog();

Q_SIGNALS:
    void eventsInserted(int requestId, const QList<quint32> &eventIds);
    void eventsFound(int requestId, const QZeitgeist::ResultSet &resultSet);
    void eventIdsFound(int requestId, const QList<quint32> &eventIds);
    void eventsGotById(int requestId, const QZeitgeist::ResultSet &resultSet);
    void eventsDeleted(int requestId, const QZeitgeist::TimeRange &timeRange);
    void relatedUrlsFound(int requestId, const QList<QUrl> &urls);
    void monitorRemoved(int requestId);

    void error(int requestId, const QString &errorMessage);

private:
    Q_DISABLE_COPY(Log)

    class LogPrivate *d;
    friend class LogPrivate;
};

} // namespace QZeitgeist

Q_DECLARE_METATYPE(QList<QUrl>)
Q_DECLARE_METATYPE(QList<quint32>)

#endif // QZEITGEIST_LOG_H_
