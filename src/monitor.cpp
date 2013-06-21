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

static void on_events_deleted(ZeitgeistMonitor *, ZeitgeistTimeRange *time_range,
                              guint32 *event_ids, int events_length, gpointer user_data)
{
    Monitor *monitor = static_cast<Monitor *>(user_data);

    TimeRange tr = TimeRange::fromHandle(time_range);
    QList<quint32> ids;

    for (int i = 0; i < events_length; ++i) {
        ids.append(event_ids[i]);
    }

    Q_EMIT monitor->eventsDeleted(tr, ids);
}

static void on_events_inserted(ZeitgeistMonitor *, ZeitgeistTimeRange *time_range,
                               ZeitgeistResultSet *events, gpointer user_data)
{
    Monitor *monitor = static_cast<Monitor *>(user_data);

    TimeRange tr = TimeRange::fromHandle(time_range);
    ResultSet resultSet = ResultSet::fromHandle(events);

    Q_EMIT monitor->eventsInserted(tr, resultSet);
}

Monitor::Monitor(const TimeRange &timeRange, const QList<Event> &eventTemplates, QObject *parent)
    : QObject(parent)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)timeRange.createHandle();
    GPtrArray *templates = Tools::eventsToPtrArray(eventTemplates);

    ZeitgeistMonitor *monitor = zeitgeist_monitor_new(tr, templates);
    g_signal_connect(monitor, "events-deleted", G_CALLBACK(on_events_deleted), this);
    g_signal_connect(monitor, "events-inserted", G_CALLBACK(on_events_inserted), this);

    g_object_unref(tr);
    g_ptr_array_unref(templates);

    m_handle = monitor;
    Q_ASSERT(m_handle);
}

Monitor::~Monitor()
{
    g_object_unref(m_handle);
}

QList<Event> Monitor::eventTemplates() const
{
    GPtrArray *templates = zeitgeist_monitor_get_event_templates((ZeitgeistMonitor *)m_handle);

    return Tools::eventsFromPtrArray(templates);
}

void Monitor::setEventTemplates(const QList<Event> &eventTemplates)
{
    GPtrArray *templates = Tools::eventsToPtrArray(eventTemplates);
    zeitgeist_monitor_set_event_templates((ZeitgeistMonitor *)m_handle, templates);

    g_ptr_array_unref(templates);
}

TimeRange Monitor::timeRange() const
{
    ZeitgeistTimeRange *tr = zeitgeist_monitor_get_time_range((ZeitgeistMonitor *)m_handle);

    return TimeRange::fromHandle(tr);
}

void Monitor::setTimeRange(const TimeRange &timeRange)
{
    ZeitgeistTimeRange *tr = (ZeitgeistTimeRange *)timeRange.createHandle();
    zeitgeist_monitor_set_time_range((ZeitgeistMonitor *)m_handle, tr);

    g_object_unref(tr);
}

HANDLE Monitor::getHandle() const
{
    return m_handle;
}

} // namespace QZeitgeist

