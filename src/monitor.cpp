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

#include "monitor.h"
#include "resultset.h"
#include "tools.h"

namespace QZeitgeist
{

class MonitorPrivate
{
public:
    MonitorPrivate(const TimeRange &timeRange, const QList<Event> &eventTemplates, Monitor *q_);
    ~MonitorPrivate();

    void emitEventsDeleted(const TimeRange &tr, const QList<quint32> &ids);
    void emitEventsInserted(const TimeRange &tr, const ResultSet &res);

    Monitor *q;
    ZeitgeistMonitor *monitor;
};

// callbacks
static void on_events_deleted(ZeitgeistMonitor *, ZeitgeistTimeRange *time_range,
                              guint32 *event_ids, int events_length, MonitorPrivate *monitor)
{
    TimeRange tr = Tools::timeRangeFromHandle(time_range);
    QList<quint32> ids;

    for (int i = 0; i < events_length; ++i) {
        ids.append(event_ids[i]);
    }

    monitor->emitEventsDeleted(tr, ids);
}

static void on_events_inserted(ZeitgeistMonitor *, ZeitgeistTimeRange *time_range,
                               ZeitgeistResultSet *events, MonitorPrivate *monitor)
{
    TimeRange tr = Tools::timeRangeFromHandle(time_range);
    ResultSet resultSet = ResultSet::fromHandle(events);

    monitor->emitEventsInserted(tr, resultSet);
}

MonitorPrivate::MonitorPrivate(const TimeRange &timeRange, const QList<Event> &eventTemplates, Monitor *q_)
    : q(q_)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)Tools::timeRangeCreateHandle(timeRange);
    GPtrArray *templates = Tools::eventsToPtrArray(eventTemplates);

    monitor = zeitgeist_monitor_new(tr, templates);
    g_signal_connect(monitor, "events-deleted", G_CALLBACK(on_events_deleted), this);
    g_signal_connect(monitor, "events-inserted", G_CALLBACK(on_events_inserted), this);

    // Don't unref templates, zeitgeist_monitor_new already takes care of it
    g_object_unref(tr);

    Q_ASSERT(monitor);
}

MonitorPrivate::~MonitorPrivate()
{
    if (monitor) {
        g_object_unref(monitor);
    }
}

void MonitorPrivate::emitEventsDeleted(const TimeRange &tr, const QList<quint32> &ids)
{
    Q_EMIT q->eventsDeleted(tr, ids);
}

void MonitorPrivate::emitEventsInserted(const TimeRange &tr, const ResultSet &res)
{
    Q_EMIT q->eventsInserted(tr, res);
}

// class Monitor
Monitor::Monitor(const TimeRange &timeRange, const QList<Event> &eventTemplates, QObject *parent)
    : QObject(parent)
    , d(new MonitorPrivate(timeRange, eventTemplates, this))
{
}

Monitor::~Monitor()
{
    delete d;
}

QList<Event> Monitor::eventTemplates() const
{
    GPtrArray *templates = zeitgeist_monitor_get_event_templates(d->monitor);

    return Tools::eventsFromPtrArray(templates);
}

void Monitor::setEventTemplates(const QList<Event> &eventTemplates)
{
    GPtrArray *templates = Tools::eventsToPtrArray(eventTemplates);
    zeitgeist_monitor_set_event_templates(d->monitor, templates);

    g_ptr_array_unref(templates);
}

TimeRange Monitor::timeRange() const
{
    ZeitgeistTimeRange *tr = zeitgeist_monitor_get_time_range(d->monitor);

    return Tools::timeRangeFromHandle(tr);
}

void Monitor::setTimeRange(const TimeRange &timeRange)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)Tools::timeRangeCreateHandle(timeRange);
    zeitgeist_monitor_set_time_range(d->monitor, tr);

    g_object_unref(tr);
}

HANDLE Monitor::getHandle() const
{
    return d->monitor;
}

void Monitor::clearHandle()
{
    d->monitor = 0;
}

} // namespace QZeitgeist

